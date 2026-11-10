/* Cable (character 17): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_17      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_17     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl17.c) says which group
                   and number each move uses.
   pl17_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl17_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl17_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl17_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_17
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_17, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_17 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl17_caught_00[], pl17_caught_01[], pl17_caught_02[], pl17_caught_03[],
	pl17_caught_04[], pl17_caught_05[], pl17_caught_06[], pl17_caught_07[], pl17_caught_08[],
	pl17_caught_09[], pl17_caught_10[], pl17_caught_11[], pl17_caught_12[], pl17_caught_13[],
	pl17_caught_14[], pl17_caught_15[], pl17_caught_16[], pl17_caught_17[], pl17_caught_18[],
	pl17_caught_19[], pl17_caught_20[], pl17_caught_21[], pl17_stand_00[], pl17_stand_02[],
	pl17_stand_03[], pl17_jump_00[], pl17_jump_00_loop[], pl17_jump_01[], pl17_jump_01_loop[],
	pl17_jump_02[], pl17_jump_02_loop[], pl17_jump_06[], pl17_jump_09[], pl17_jump_03[],
	pl17_dash_00[], pl17_dash_00_loop[], pl17_dash_02[], pl17_dash_02_loop[], pl17_dash_01[],
	pl17_dash_03[], pl17_crouch_00[], pl17_crouch_01[], pl17_crouch_02[], pl17_turn_00[],
	pl17_turn_01[], pl17_guard_00[], pl17_guard_01[], pl17_guard_02[], pl17_guard_03[],
	pl17_guard_04[], pl17_guard_05[], pl17_guard_06[], pl17_guard_07[], pl17_stand_p_00[],
	pl17_stand_p_01[], pl17_stand_p_02[], pl17_stand_p_03[], pl17_stand_p_04[],
	pl17_stand_p_04_loop[], pl17_stand_k_00[], pl17_stand_k_01[], pl17_stand_k_02[],
	pl17_stand_k_03[], pl17_air_guard_00[], pl17_air_guard_01[], pl17_crouch_p_00[],
	pl17_crouch_p_01[], pl17_crouch_p_02[], pl17_crouch_p_02_loop[], pl17_crouch_k_00[],
	pl17_crouch_k_01[], pl17_crouch_k_02[], pl17_jump_p_00[], pl17_jump_p_01[], pl17_jump_p_02[],
	pl17_jump_k_00[], pl17_jump_k_01[], pl17_jump_k_02[], pl17_get_up_00[], pl17_get_up_01[],
	pl17_get_up_02[], pl17_get_up_03[], pl17_damage_02[], pl17_damage_01[], pl17_damage_00[],
	pl17_damage_00_loop[], pl17_damage_05[], pl17_damage_04[], pl17_damage_03[],
	pl17_damage_03_loop[], pl17_damage_08[], pl17_damage_07[], pl17_damage_06[], pl17_damage_10[],
	pl17_damage_11[], pl17_damage_12[], pl17_damage_13[], pl17_damage_13_loop[], pl17_damage_09[],
	pl17_damage_09_loop[], pl17_damage_16[], pl17_damage_17[], pl17_damage_18[], pl17_damage_19[],
	pl17_damage_20[], pl17_damage_21[], pl17_damage_22[], pl17_damage_23[], pl17_damage_24[],
	pl17_damage_25[], pl17_damage_26[], pl17_damage_27[], pl17_damage_32[], pl17_damage_33[],
	pl17_damage_30[], pl17_damage_31[], pl17_damage_28[], pl17_damage_29[], pl17_special_00[],
	pl17_special_01[], pl17_special_02[], pl17_special_03[], pl17_special_04[], pl17_special_05[],
	pl17_special_06[], pl17_special_07[], pl17_special_08[], pl17_special_09[], pl17_special_10[],
	pl17_special_11[], pl17_special_11_loop[], pl17_special_12[], pl17_special_12_loop[],
	pl17_special_13[], pl17_special_14[], pl17_special_15[], pl17_special_16[],
	pl17_special_16_loop[], pl17_special_17[], pl17_lance_land_h_lp[], pl17_cable_punch_start[],
	pl17_cable_punch_start_loop[], pl17_cable_punch_finish_l[], pl17_cable_punch_finish_h[],
	pl17_special_21[], pl17_special_22[], pl17_special_23[], pl17_super_00[], pl17_super_01[],
	pl17_super_02[], pl17_super_03[], pl17_super_04[], pl17_super_05[], pl17_super_05_loop[],
	pl17_super_06[], pl17_super_06_loop[], pl17_super_07[], pl17_effect_00[], pl17_effect_05[],
	pl17_effect_01[], pl17_effect_06[], pl17_effect_02[], pl17_effect_07[], pl17_effect_10[],
	pl17_effect_13[], pl17_effect_14[], pl17_catch_02[], pl17_catch_02_loop[], pl17_effect_03[],
	pl17_effect_08[], pl17_effect_04[], pl17_effect_09[], pl17_effect_11[], pl17_effect_12[],
	pl17_effect_19[], pl17_effect_18[], pl17_effect_16[], pl17_effect_17[], pl17_effect_20[],
	pl17_catch_00[], pl17_catch_00_loop[], pl17_catch_03[], pl17_catch_03_loop[], pl17_catch_01[],
	pl17_catch_01_loop[], pl17_change_00[], pl17_change_00_loop[], pl17_change_01[],
	pl17_change_02[], pl17_change_02_loop[], pl17_change_03[], pl17_change_03_loop[],
	pl17_change_04[], pl17_extra_00[], pl17_extra_01[], pl17_extra_01_loop[], pl17_extra_02[],
	pl17_extra_02_loop[], pl17_extra_03[], pl17_extra_04[], pl17_extra_05[], pl17_dizzy_00[],
	pl17_win_00[], pl17_win_00_loop[], pl17_win_01[], pl17_win_01_loop[], pl17_win_02[],
	pl17_win_02_loop[], pl17_appear_00[], pl17_appear_01[], pl17_appear_01_loop[], pl17_grp27_02[],
	pl17_grp27_03[], pl17_grp27_04[], pl17_grp27_05[], pl17_grp27_06[], pl17_grp27_00[],
	pl17_grp27_01[], pl17_grp27_07[], pl17_grp27_08[], pl17_grp27_09[], pl17_grp27_10[],
	pl17_grp27_11[], pl17_grp27_12[], pl17_grp27_13[], pl17_grp27_14[], pl17_grp27_15[],
	pl17_grp27_16[], pl17_grp27_17[], pl17_grp27_18[], pl17_grp27_19[], pl17_grp27_20[],
	pl17_grp27_21[], pl17_grp27_22[], pl17_grp27_23[], pl17_grp27_24[], pl17_grp27_25[],
	pl17_grp27_26[], pl17_grp27_27[], pl17_grp27_28[];
extern const Sint32 *const pl17_stand_tbl[], *const pl17_jump_tbl[], *const pl17_dash_tbl[],
	*const pl17_crouch_tbl[], *const pl17_turn_tbl[], *const pl17_guard_tbl[],
	*const pl17_air_guard_tbl[], *const pl17_stand_p_tbl[], *const pl17_stand_k_tbl[],
	*const pl17_crouch_p_tbl[], *const pl17_crouch_k_tbl[], *const pl17_jump_p_tbl[],
	*const pl17_jump_k_tbl[], *const pl17_damage_tbl[], *const pl17_dizzy_tbl[],
	*const pl17_catch_tbl[], *const pl17_caught_tbl[], *const pl17_get_up_tbl[],
	*const pl17_appear_tbl[], *const pl17_win_tbl[], *const pl17_extra_tbl[],
	*const pl17_special_tbl[], *const pl17_super_tbl[], *const pl17_effect_tbl[],
	*const pl17_change_tbl[], *const pl17_grp25_tbl[], *const pl17_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_17[16] = {
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
const Sint32 *const *const char_tbl_17[] = {
	pl17_stand_tbl, pl17_jump_tbl, pl17_dash_tbl, pl17_crouch_tbl, pl17_turn_tbl, pl17_guard_tbl,
	pl17_air_guard_tbl, pl17_stand_p_tbl, pl17_stand_k_tbl, pl17_crouch_p_tbl, pl17_crouch_k_tbl, pl17_jump_p_tbl,
	pl17_jump_k_tbl, pl17_damage_tbl, pl17_dizzy_tbl, pl17_catch_tbl, pl17_caught_tbl, pl17_get_up_tbl,
	pl17_appear_tbl, pl17_win_tbl, pl17_extra_tbl, pl17_special_tbl, pl17_super_tbl, pl17_effect_tbl,
	pl17_change_tbl, pl17_grp25_tbl, pl17_grp25_tbl, pl17_grp27_tbl, 0,
};

const Sint32 *const pl17_stand_tbl[] = {
	pl17_stand_00, pl17_stand_00, pl17_stand_02, pl17_stand_03, pl17_stand_00, pl17_stand_00,
	pl17_stand_02, pl17_stand_03, 0,
};

const Sint32 *const pl17_jump_tbl[] = {
	pl17_jump_00, pl17_jump_01, pl17_jump_02, pl17_jump_03, pl17_jump_03, pl17_jump_03,
	pl17_jump_06, pl17_jump_06, pl17_jump_06, pl17_jump_09, pl17_jump_09, pl17_jump_09,
	0,
};

const Sint32 *const pl17_dash_tbl[] = {
	pl17_dash_00, pl17_dash_01, pl17_dash_02, pl17_dash_03, 0,
};

const Sint32 *const pl17_crouch_tbl[] = {
	pl17_crouch_00, pl17_crouch_01, pl17_crouch_02, 0,
};

const Sint32 *const pl17_turn_tbl[] = {
	pl17_turn_00, pl17_turn_01, 0,
};

const Sint32 *const pl17_guard_tbl[] = {
	pl17_guard_00, pl17_guard_01, pl17_guard_02, pl17_guard_03, pl17_guard_04, pl17_guard_05,
	pl17_guard_06, pl17_guard_07, 0,
};

const Sint32 *const pl17_air_guard_tbl[] = {
	pl17_air_guard_00, pl17_air_guard_01, pl17_air_guard_01, 0,
};

const Sint32 *const pl17_stand_p_tbl[] = {
	pl17_stand_p_00, pl17_stand_p_01, pl17_stand_p_02, pl17_stand_p_03, pl17_stand_p_04, 0,
};

const Sint32 *const pl17_stand_k_tbl[] = {
	pl17_stand_k_00, pl17_stand_k_01, pl17_stand_k_02, pl17_stand_k_03, 0,
};

const Sint32 *const pl17_crouch_p_tbl[] = {
	pl17_crouch_p_00, pl17_crouch_p_01, pl17_crouch_p_02, 0,
};

const Sint32 *const pl17_crouch_k_tbl[] = {
	pl17_crouch_k_00, pl17_crouch_k_01, pl17_crouch_k_02, 0,
};

const Sint32 *const pl17_jump_p_tbl[] = {
	pl17_jump_p_00, pl17_jump_p_01, pl17_jump_p_02, pl17_jump_p_00, pl17_jump_p_01, pl17_jump_p_02,
	0,
};

const Sint32 *const pl17_jump_k_tbl[] = {
	pl17_jump_k_00, pl17_jump_k_01, pl17_jump_k_02, pl17_jump_k_00, pl17_jump_k_01, pl17_jump_k_02,
	0,
};

const Sint32 *const pl17_damage_tbl[] = {
	pl17_damage_00, pl17_damage_01, pl17_damage_02, pl17_damage_03, pl17_damage_04, pl17_damage_05,
	pl17_damage_06, pl17_damage_07, pl17_damage_08, pl17_damage_09, pl17_damage_10, pl17_damage_11,
	pl17_damage_12, pl17_damage_13, pl17_damage_13, pl17_damage_13, pl17_damage_16, pl17_damage_17,
	pl17_damage_18, pl17_damage_19, pl17_damage_20, pl17_damage_21, pl17_damage_22, pl17_damage_23,
	pl17_damage_24, pl17_damage_25, pl17_damage_26, pl17_damage_27, pl17_damage_28, pl17_damage_29,
	pl17_damage_30, pl17_damage_31, pl17_damage_32, pl17_damage_33, pl17_damage_13, 0,
};

const Sint32 *const pl17_dizzy_tbl[] = {
	pl17_dizzy_00, 0,
};

const Sint32 *const pl17_catch_tbl[] = {
	pl17_catch_00, pl17_catch_01, pl17_catch_02, pl17_catch_03, 0,
};

const Sint32 *const pl17_caught_tbl[] = {
	pl17_caught_00, pl17_caught_01, pl17_caught_02, pl17_caught_03, pl17_caught_04, pl17_caught_05,
	pl17_caught_06, pl17_caught_07, pl17_caught_08, pl17_caught_09, pl17_caught_10, pl17_caught_11,
	pl17_caught_12, pl17_caught_13, pl17_caught_14, pl17_caught_15, pl17_caught_16, pl17_caught_17,
	pl17_caught_18, pl17_caught_19, pl17_caught_20, pl17_caught_21, 0,
};

const Sint32 *const pl17_get_up_tbl[] = {
	pl17_get_up_00, pl17_get_up_01, pl17_get_up_02, pl17_get_up_03, 0,
};

const Sint32 *const pl17_appear_tbl[] = {
	pl17_appear_00, pl17_appear_01, 0,
};

const Sint32 *const pl17_win_tbl[] = {
	pl17_win_00, pl17_win_01, pl17_win_02, 0,
};

const Sint32 *const pl17_extra_tbl[] = {
	pl17_extra_00, pl17_extra_01, pl17_extra_02, pl17_extra_03, pl17_extra_04, pl17_extra_05,
	0,
};

const Sint32 *const pl17_special_tbl[] = {
	pl17_special_00, pl17_special_01, pl17_special_02, pl17_special_03, pl17_special_04, pl17_special_05,
	pl17_special_06, pl17_special_07, pl17_special_08, pl17_special_09, pl17_special_10, pl17_special_11,
	pl17_special_12, pl17_special_13, pl17_special_14, pl17_special_15, pl17_special_16, pl17_special_17,
	pl17_cable_punch_start, pl17_cable_punch_finish_l, pl17_cable_punch_finish_h, pl17_special_21, pl17_special_22, pl17_special_23,
	0,
};

const Sint32 *const pl17_super_tbl[] = {
	pl17_super_00, pl17_super_01, pl17_super_02, pl17_super_03, pl17_super_04, pl17_super_05,
	pl17_super_06, pl17_super_07, 0,
};

const Sint32 *const pl17_effect_tbl[] = {
	pl17_effect_00, pl17_effect_01, pl17_effect_02, pl17_effect_03, pl17_effect_04, pl17_effect_05,
	pl17_effect_06, pl17_effect_07, pl17_effect_08, pl17_effect_09, pl17_effect_10, pl17_effect_11,
	pl17_effect_12, pl17_effect_13, pl17_effect_14, pl17_catch_02, pl17_effect_16, pl17_effect_17,
	pl17_effect_18, pl17_effect_19, pl17_effect_20, 0,
};

const Sint32 *const pl17_change_tbl[] = {
	pl17_change_00, pl17_change_01, pl17_change_02, pl17_change_03, pl17_change_04, 0,
};

const Sint32 *const pl17_grp25_tbl[] = {
	pl17_stand_00, 0,
};

const Sint32 *const pl17_grp27_tbl[] = {
	pl17_grp27_00, pl17_grp27_01, pl17_grp27_02, pl17_grp27_03, pl17_grp27_04, pl17_grp27_05,
	pl17_grp27_06, pl17_grp27_07, pl17_grp27_08, pl17_grp27_09, pl17_grp27_10, pl17_grp27_11,
	pl17_grp27_12, pl17_grp27_13, pl17_grp27_14, pl17_grp27_15, pl17_grp27_16, pl17_grp27_17,
	pl17_grp27_18, pl17_grp27_19, pl17_grp27_20, pl17_grp27_21, pl17_grp27_22, pl17_grp27_23,
	pl17_grp27_24, pl17_grp27_25, pl17_grp27_26, pl17_grp27_27, pl17_grp27_28, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Caught 0 */
const Sint32 pl17_caught_00[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   500, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl17_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl17_caught_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   501, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl17_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl17_caught_02[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   502, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl17_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl17_caught_03[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   504, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl17_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl17_caught_04[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   505, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl17_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl17_caught_05[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   506, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl17_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl17_caught_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   507, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl17_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl17_caught_07[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   508, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl17_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl17_caught_08[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   509, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl17_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl17_caught_09[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   504, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl17_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl17_caught_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   505, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl17_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl17_caught_11[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   506, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl17_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl17_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   515, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl17_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl17_caught_13[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   519, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl17_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl17_caught_14[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   521, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl17_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl17_caught_15[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   522, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl17_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl17_caught_16[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   523, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl17_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl17_caught_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   524, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl17_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl17_caught_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   530, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl17_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl17_caught_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   536, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl17_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl17_caught_20[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   529, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl17_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl17_caught_21[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   516, 0x80,   0x0c,    0,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl17_caught_21 ),
};

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   Char_Fly 0 */
const Sint32 pl17_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,    40, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    41, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    42, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    43, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    44, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    45, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    46, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    48, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    50, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    51, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    52, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    53, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    54, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    55, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    56, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    57, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    59, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    60, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    61, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    62, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    63, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    40, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    41, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    42, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    43, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    44, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    45, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    46, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    48, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    50, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    51, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    52, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    53, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    54, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    55, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    56, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    57, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    59, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    60, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    61, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    62, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    63, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    82, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    85, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    86, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    88, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    89, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    90, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl17_stand_02[] = {
	CG_FRAME(    0,    0,   6,           0,    91, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    92, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    93, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    94, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    95, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    96, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    97, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    98, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    99, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   100, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   101, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   102, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl17_stand_03[] = {
	CG_FRAME(    0,    0,   7,           0,   103, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   104, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   105, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   106, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   107, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   108, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   109, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   110, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   111, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   112, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   113, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   114, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl17_jump_00[] = {
	CG_FRAME(    0,    1,   2,           0,   132, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   1,           0,   130, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   1,           0,   131, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   125, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   128, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl17_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   129, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl17_jump_01[] = {
	CG_FRAME(    0,    1,   2,           0,   132, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   1,           0,   130, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   1,           0,   131, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   138, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl17_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   140, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl17_jump_02[] = {
	CG_FRAME(    0,    1,   2,           0,   132, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   1,           0,   130, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   1,           0,   131, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   138, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl17_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   141, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_jump_02_loop ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl17_jump_06[] = {
	CG_FRAME(    0,    1,   2,           0,   132, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   1,           0,   130, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   1,           0,   131, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   128, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl17_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   164, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_jump_09 ),
};

/* Char_Jump 3: app01_fall
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl17_jump_03[] = {
	CG_FRAME(    0,    1,   1,           0,   130, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,   130, 0x80,   0x11,    0,  0x00, 0x34 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8,           0,   131, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8,           0,   132, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8, END_OF_DATA,   133, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_jump_03 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl17_dash_00[] = {
	CG_FRAME(    0,    1,   1,           0,   119, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   1,           0,   118, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   1,           0,   117, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   1,           0,   116, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl17_dash_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   115, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl17_dash_00_loop ),
};

/* Char_Dash 2: shl17_05_fly */
const Sint32 pl17_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_dash_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   120, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_dash_02_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl17_dash_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   121, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl17_dash_01 ),
};

/* Char_Dash 3 */
const Sint32 pl17_dash_03[] = {
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   120, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl17_crouch_00[] = {
	CG_FRAME(    0,    0,   3,           0,   146, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   147, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   148, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl17_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl17_crouch_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   145, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl17_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl17_crouch_02[] = {
	CG_FRAME(    0,    0,   3,           0,   148, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   147, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   146, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl17_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl17_turn_00[] = {
	CG_FRAME(    0,    0,   6,           0,   149, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   150, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   151, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl17_turn_01[] = {
	CG_FRAME(    0,    0,   6,           0,   152, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   153, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   154, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl17_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl17_guard_00[] = {
	CG_FRAME(    0,    0,   6,           0,   156, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   155, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl17_guard_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   155, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl17_guard_02[] = {
	CG_FRAME(    0,    0,   6,           0,   155, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   156, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl17_guard_03[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   155, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl17_guard_04[] = {
	CG_FRAME(    0,    0,   6,           0,   158, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   157, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl17_guard_05[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   157, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl17_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl17_guard_06[] = {
	CG_FRAME(    0,    0,   6,           0,   157, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   158, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   159, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl17_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl17_guard_07[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   157, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl17_guard_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl17_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   251, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x11,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   5,           0,   253, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   254, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl17_stand_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x11,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   257, 0x80,   0x11,  ATK,  0x00,       0x03,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   258, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   260, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl17_stand_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   261, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   262, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   263, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   2,           0,   264, 0x80,   0x0a,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   8,           0,   265, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
};

/* Char_Stand_P 3: hp_loop */
const Sint32 pl17_stand_p_03[] = {
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    2,   2,           0,   265, 0x80,   0x0a,  ATK,  0x00,       0x24,    0,   0,     0, 0x0100,  18 ),
	CG_FRAME(    0,    3,   2,           0,   266, 0x80,   0x0a,  ATE,  0x00,          0,    0,   0,     0, 0x0200,  19 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x0a,  ATE,  0x00,          0,    0,   0,     0, 0x0300,  19 ),
	CG_FRAME(    0,    0,   2,           0,   268, 0x80,   0x0a,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    4,   2,           0,   265, 0x80,   0x0a,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
};

/* Char_Stand_P 4 */
const Sint32 pl17_stand_p_04[] = {
	CG_FRAME(    0,    0,  10,           0,   265, 0x80,   0x0a,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   5,           0,   269, 0x80,   0x0a,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_stand_p_04_loop[] = {
	CG_FRAME(    0,    5,  47,           0,   270, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    5,   8,           0,   271, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    5,  36,           0,   272, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    5,   8, END_OF_DATA,   271, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_stand_p_04_loop ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl17_stand_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   274, 0x80,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   275, 0x80,   0x06,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   6,           0,   276, 0x80,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   274, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   277, 0x80,   0x0d,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl17_stand_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   278, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   1,           0,   279, 0x80,   0x06,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  22 ),
	CG_FRAME( 0x80,    0,   1,           0,   280, 0x80,   0x01,  ATK,  0x00,       0x03,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   281, 0x80,   0x01,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,  10,           0,   280, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0,   282, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0,   283, 0x80,   0x05,  ATE,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   284, 0x80,   0x0d,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl17_stand_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   285, 0x80,   0x08,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   286, 0x80,   0x08,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   287, 0x80,   0x04,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   3,           0,   288, 0x80,   0x04,  ATK,  0x00,       0x04,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   7,           0,   288, 0x80,   0x04,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   5,           0,   289, 0x80,   0x04,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4,           0,   291, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   277, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl17_stand_k_02 ),
};

/* Char_Stand_K 3 */
const Sint32 pl17_stand_k_03[] = {
	CG_FRAME(    0,    1,   3,           0,   385, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   386, 0x80,   0x02,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    2,   2,           0,   387, 0x80,   0x21,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,  10,           0,   388, 0x80,   0x21,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    3,   3,           0,   389, 0x80,   0x01,  ATK,  0x00,       0x04,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    4,  10,           0,   390, 0x80,   0x01,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    4,   5,           0,   390, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   5,           0,   391, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   392, 0x80,   0x03,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   393, 0x80,   0x0e,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_stand_k_03 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl17_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl17_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   160, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_air_guard_01 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl17_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   292, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   293, 0x80,   0x11,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   6,           0,   294, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   295, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl17_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl17_crouch_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   296, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x11,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   298, 0x80,   0x11,  ATK,  0x00,       0x03,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   8,           0,   299, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   6,           0,   300, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   301, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl17_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl17_crouch_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   302, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x80,   0x0d,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x0b,  ATK,  0x00,       0x04,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0,   305, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
};

const Sint32 pl17_crouch_p_02_loop[] = {
	CG_FRAME(    0,    0,   5,           0,   305, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    2,   2,           0,   305, 0x80,   0x0c,  ATK,  0x00,       0x24,    0,   0,     0, 0x0100,  58 ),
	CG_FRAME(    0,    3,   3,           0,   306, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0, 0x0200,  37 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0, 0x0300,  37 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   8,           0,   305, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   5,           0,   309, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   310, 0x80,   0x0d,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   311, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl17_crouch_p_02_loop ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl17_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   312, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   313, 0x80,   0x69,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   8,           0,   314, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   312, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   316, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl17_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl17_crouch_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   312, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   317, 0x80,   0x0f,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   318, 0x80,   0x56,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   319, 0x80,   0x56,  ATK,  0x00,       0x03,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,  10,           0,   320, 0x80,   0x56,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   5,           0,   321, 0x80,   0x56,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   4,           0,   322, 0x80,   0x12,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   312, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl17_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl17_crouch_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0x45,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   3,           0,   326, 0x80,   0x41,  ATK,  0x00,       0x03,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   8,           0,   326, 0x80,   0x41,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x07,  ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4,           0,   329, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   330, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl17_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl17_jump_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   331, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   331, 0x80,   0x11,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   332, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   8,           0,   333, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   5,           0,   333, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   126, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl17_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl17_jump_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   334, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   335, 0x80,   0x11,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   336, 0x80,   0x11,  ATK,  0x00,       0x03,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   8,           0,   337, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   339, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   129, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl17_jump_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   340, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  12,           0,   341, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    2,   2,           0,   341, 0x80,   0x11,  ATK,  0x00,       0x24,    0,   0,     0, 0x0100,  49 ),
	CG_FRAME(    0,    3,   2,           0,   342, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0200,  50 ),
	CG_FRAME(    0,    0,  12,           0,   343, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0300,  50 ),
	CG_FRAME(    0,    0,   6,           0,   344, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   340, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl17_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl17_jump_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   345, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   346, 0x80,   0x11,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   3,           0,   347, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   8,           0,   348, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   6,           0,   349, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   340, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl17_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl17_jump_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,   350, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   351, 0x80,   0x11,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   352, 0x80,   0x11,  ATK,  0x00,       0x03,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,  10,           0,   353, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   5,           0,   354, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   355, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl17_jump_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   356, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   9,           0,   357, 0x80,   0x11,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   358, 0x80,   0x11,  ATK,  0x00,       0x04,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4,           0,   359, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   8,           0,   359, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   5,           0,   360, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   361, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl17_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl17_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   168, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   133, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl17_get_up_01[] = {
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   172, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   173, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   174, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl17_get_up_02[] = {
	CG_FRAME(    0,    0,   7,           0,   169, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   170, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   171, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   172, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   173, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   174, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl17_get_up_03[] = {
	CG_FRAME(    0,    0,   5,           0,   169, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   174, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   173, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   172, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   171, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   170, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_get_up_03 ),
};

/* Char_Damage 2 */
const Sint32 pl17_damage_02[] = {
	CG_FRAME(    0,    0,   6,           0,   502, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Damage 1 */
const Sint32 pl17_damage_01[] = {
	CG_FRAME(    0,    0,   7,           0,   501, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl17_damage_00[] = {
	CG_FRAME(    0,    0,   6,           0,   500, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_damage_00_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   499, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_damage_00_loop ),
};

/* Char_Damage 5 */
const Sint32 pl17_damage_05[] = {
	CG_FRAME(    0,    0,   6,           0,   506, 0x80,   0x18,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Damage 4 */
const Sint32 pl17_damage_04[] = {
	CG_FRAME(    0,    0,   7,           0,   505, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Damage 3 */
const Sint32 pl17_damage_03[] = {
	CG_FRAME(    0,    0,   6,           0,   504, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_damage_03_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   503, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_damage_03_loop ),
};

/* Char_Damage 8 */
const Sint32 pl17_damage_08[] = {
	CG_FRAME(    0,    0,   6,           0,   509, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 7 */
const Sint32 pl17_damage_07[] = {
	CG_FRAME(    0,    0,   7,           0,   508, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 6 */
const Sint32 pl17_damage_06[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   507, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl17_damage_06 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl17_damage_10[] = {
	CG_FRAME(    0,    0,   5,           0,   511, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   510, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl17_damage_11[] = {
	CG_FRAME(    0,    0,   4,           0,   512, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   511, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   510, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl17_damage_12[] = {
	CG_FRAME(    0,    0,   3,           0,   513, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   511, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   510, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl17_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   514, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   528, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl17_damage_13_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   529, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl17_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl17_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   516, 0x80,   0x0c,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   517, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   518, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   526, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   527, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   528, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl17_damage_09_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   529, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl17_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl17_damage_16[] = {
	CG_FRAME(    0,    0,   3,           0,   519, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   520, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl17_damage_17[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   521, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl17_damage_18[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   522, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl17_damage_19[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   523, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl17_damage_20[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   524, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl17_damage_21[] = {
	CG_FRAME(    0,    0,   3,           0,   519, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   520, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl17_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   521, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl17_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   522, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl17_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   523, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl17_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   524, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl17_damage_26[] = {
	CG_FRAME(    0,    0,   3,           0,   525, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   526, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   527, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   528, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl17_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl17_damage_27[] = {
	CG_FRAME(    0,    0,   4,           0,   522, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   174, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   173, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   172, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   125, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_27 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl17_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,   544, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   543, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   542, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   547, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   546, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   125, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl17_damage_33[] = {
	CG_FRAME(    0,    0,   4,           0,   174, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   173, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   172, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl17_damage_33 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl17_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   521, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   522, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   547, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   546, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   545, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   125, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl17_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   529, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl17_damage_31 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl17_damage_28[] = {
	CG_FRAME(    0,    0,   2,           0,   530, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   531, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   532, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   533, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   534, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   535, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl17_damage_29[] = {
	CG_FRAME(    0,    0,   2,           0,   536, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   537, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   538, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   539, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   540, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   541, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_damage_29 ),
};

/* Char_Special 0 */
const Sint32 pl17_special_00[] = {
	CG_FRAME(    0,    0,   3,           0,   394, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4,           0,   395, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   6,           0,   396, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   6,           0,   397, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   6,           0,   398, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   6,           0,   399, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   4,           0,   401, 0x80,   0x4b,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   402, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   403, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl17_special_00 ),
};

/* Char_Special 1 */
const Sint32 pl17_special_01[] = {
	CG_FRAME(    0,    0,   6,           0,   404, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   6,           0,   405, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,  10,           0,   406, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    1,  14,           0,   407, 0x80,   0x11,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,   408, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl17_special_01 ),
};

/* Char_Special 2: viper_init (Viper Beam) */
const Sint32 pl17_special_02[] = {
	CG_FRAME(    0,    0,   6,           0,   409, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  77 ),
	CG_FRAME(    0,    0,   3,           0,   410, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0200,  77 ),
	CG_FRAME(    0,    0,   3,           0,   411, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0300,  77 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   412, 0x80,   0x4e,  ATK,  0x00,       0x14,    0,   0,     0, 0x0400,  78 ),
	CG_NEXT( pl17_special_02 ),
};

/* Char_Special 3: viper_init (Viper Beam) */
const Sint32 pl17_special_03[] = {
	CG_FRAME(    0,    0,   7,           0,   409, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  77 ),
	CG_FRAME(    0,    0,   3,           0,   410, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0200,  77 ),
	CG_FRAME(    0,    0,   3,           0,   411, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0300,  77 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   412, 0x80,   0x4e,  ATK,  0x00,       0x14,    0,   0,     0, 0x0400,  78 ),
	CG_NEXT( pl17_special_03 ),
};

/* Char_Special 4: viper_move (Viper Beam) */
const Sint32 pl17_special_04[] = {
	CG_FRAME(    0,    1,   1,           0,   412, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0400,  78 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   413, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0500,  78 ),
	CG_NEXT( pl17_special_04 ),
};

/* Char_Special 5 */
const Sint32 pl17_special_05[] = {
	CG_FRAME(    0,    0,  16,           0,   412, 0x80,   0x4e,  ATE,  0x00,          0,    0,   0,     0, 0x0400,  78 ),
	CG_FRAME(    0,    0,   5,           0,   411, 0x80,   0x4e,  ATE,  0x00,          0,    0,   0,     0, 0x0600,  79 ),
	CG_FRAME(    0,    0,   5,           0,   410, 0x80,   0x4e,  ATE,  0x00,          0,    0,   0,     0, 0x0700,  79 ),
	CG_FRAME(    0,    0,   5,           0,   409, 0x80,   0x4e,  ATE,  0x00,          0,    0,   0,     0, 0x0800,  79 ),
	CG_FRAME(    0,    0,   5,           0,   414, 0x80,   0x4e,  ATE,  0x00,          0,    0,   0,     0, 0x0900,  79 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   415, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl17_special_05 ),
};

/* Char_Special 6: viper_init (Viper Beam) */
const Sint32 pl17_special_06[] = {
	CG_FRAME(    0,    0,   4,           0,   445, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  80 ),
	CG_FRAME(    0,    0,   4,           0,   446, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x0200,  80 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,   447, 0x80,   0x11,  ATK,  0x00,       0x14,    0,   0,     0, 0x0300,  80 ),
	CG_NEXT( pl17_special_06 ),
};

/* Char_Special 7: viper_init (Viper Beam) */
const Sint32 pl17_special_07[] = {
	CG_FRAME(    0,    0,   4,           0,   445, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  80 ),
	CG_FRAME(    0,    0,   4,           0,   446, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x0200,  80 ),
	CG_FRAME(    0,    0,  11, END_OF_DATA,   447, 0x80,   0x11,  ATK,  0x00,       0x14,    0,   0,     0, 0x0300,  80 ),
	CG_NEXT( pl17_special_07 ),
};

/* Char_Special 8: viper_move (Viper Beam) */
const Sint32 pl17_special_08[] = {
	CG_FRAME(    0,    1,   1,           0,   448, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x0400,  81 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   449, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x0500,  81 ),
	CG_NEXT( pl17_special_08 ),
};

/* Char_Special 9 */
const Sint32 pl17_special_09[] = {
	CG_FRAME(    0,    1,   9,           0,   447, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0600,  82 ),
	CG_FRAME(    0,    0,   5,           0,   446, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0700,  82 ),
	CG_FRAME(    0,    0,   5,           0,   445, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0800,  82 ),
	CG_FRAME(    0,    0,   5,           0,   450, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0900,  82 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   451, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl17_special_09 ),
};

/* Char_Special 10: psimitar_init (Psimitar) */
const Sint32 pl17_special_10[] = {
	CG_FRAME(    0,    1,   2,           0,   421, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   422, 0x80,   0x4a,  ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   423, 0x80,   0x4a,  ATK,  0x02,          0,    0,   0,     0, 0x0100,  83 ),
	CG_NEXT( pl17_special_10 ),
};

/* Char_Special 11: psimitar_move (Psimitar, 11 + atck_level) */
const Sint32 pl17_special_11[] = {
	CG_FRAME(    0,    1,   6,           0,   424, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    3,   1,           0,   425, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  85 ),
};

const Sint32 pl17_special_11_loop[] = {
	CG_FRAME(    0,    2,   2, END_OF_DATA,   425, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl17_special_11_loop ),
};

/* Char_Special 12 */
const Sint32 pl17_special_12[] = {
	CG_FRAME(    0,    1,   6,           0,   424, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    3,   1,           0,   425, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  85 ),
};

const Sint32 pl17_special_12_loop[] = {
	CG_FRAME(    0,    2,   2, END_OF_DATA,   425, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl17_special_12_loop ),
};

/* Char_Special 13: psimitar_rise (Psimitar) */
const Sint32 pl17_special_13[] = {
	CG_FRAME(    0,    0,   3,           0,   426, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   3,           0,   428, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   2,           0,   429, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   430, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl17_special_13 ),
};

/* Char_Special 14: psimitar_top (Psimitar, 14 + atck_level) */
const Sint32 pl17_special_14[] = {
	CG_FRAME(    0,    0,   2,           0,   129, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   164, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl17_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl17_special_15[] = {
	CG_FRAME(    0,    0,   2,           0,   129, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   164, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl17_special_15 ),
};

/* Char_Special 16: psimitar_fall (Psimitar, 16 + atck_level) */
const Sint32 pl17_special_16[] = {
	CG_FRAME(    0,    1,   4,           0,   130, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   131, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   132, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl17_special_16_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   133, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_special_16_loop ),
};

/* Char_Special 17 */
const Sint32 pl17_special_17[] = {
	CG_FRAME(    0,    1,   5,           0,   130, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   5,           0,   131, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   5,           0,   132, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl17_lance_land_h_lp[] = {
	CG_FRAME(    0,    1,   5, END_OF_DATA,   133, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_lance_land_h_lp ),
};

/* Char_Special 18: crackdown_init (Crackdown) */
const Sint32 pl17_cable_punch_start[] = {
	CG_FRAME(    0,    0,   5,           0,   435, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   3,           0,   436, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    3,   8,           0,   437, 0x80,   0x6c,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl17_cable_punch_start_loop[] = {
	CG_FRAME(    0,    4,   1, END_OF_DATA,   438, 0x80,   0x97,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_NEXT( pl17_cable_punch_start_loop ),
};

/* Char_Special 19: crackdown_dash (Crackdown) */
const Sint32 pl17_cable_punch_finish_l[] = {
	CG_FRAME( 0x80,    2,   2,           0,   439, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x0200,  87 ),
	CG_FRAME(    0,    3,   2,           0,   440, 0x80,   0x11,  ATE,  0x00, 0x4b | CSE,    0,   0,     0, 0x0300,  88 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0400,   5 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0500,   5 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0600,   5 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0700,   5 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0800,   5 ),
	CG_FRAME(    0,    0,   8,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0900,   5 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   443, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    4,   4, END_OF_DATA,   444, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_cable_punch_finish_l ),
};

/* Char_Special 20: crackdown_dash (Crackdown) */
const Sint32 pl17_cable_punch_finish_h[] = {
	CG_FRAME(    0,    2,   2,           0,   439, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x0200,  87 ),
	CG_FRAME(    0,    3,   2,           0,   440, 0x80,   0x11,  ATE,  0x00, 0x4b | CSE,    0,   0,     0, 0x0300,  88 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0400,   5 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0500,   5 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0600,   5 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0700,   5 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0800,   5 ),
	CG_FRAME(    0,    0,  10,           0,   441, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0, 0x0900,   5 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   443, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    4,   5, END_OF_DATA,   444, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_cable_punch_finish_h ),
};

/* Char_Special 21: psycharge_init (Psy-Charge) */
const Sint32 pl17_special_21[] = {
	CG_FRAME(    0,    0,   7,           0,   255, 0x80,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   7,           0,   256, 0x80,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   374, 0x80,   0x0e,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl17_special_21 ),
};

/* Char_Special 22: psycharge_move (Psy-Charge) */
const Sint32 pl17_special_22[] = {
	CG_FRAME(    0,    2,   2,           0,   375, 0x80,   0x0e,  ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   8,           0,   375, 0x80,   0x0e,  ATE,  0x00, 0x4f | CSE,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   384, 0x80,   0x0e,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_special_22 ),
};

/* Char_Special 23: snapback_init (the snapback) */
const Sint32 pl17_special_23[] = {
	CG_FRAME(    0,    0,  10,           0,   285, 0x80,   0x08,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,   15,   2,           0,   285, 0x80,   0x08,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   3,           0,   286, 0x80,   0x08,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   287, 0x80,   0x04,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   3,           0,   288, 0x80,   0x04,  ATK,  0x00,       0x04,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   7,           0,   288, 0x80,   0x04,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   5,           0,   289, 0x80,   0x04,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4,           0,   291, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   277, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl17_special_23 ),
};

/* Char_Super 0 */
const Sint32 pl17_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,   457, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x80,    1,   3,           0,   458, 0x80,   0x4f,  ATK,  0x01,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x80,    0,   3,           0,   459, 0x80,   0x6a,  ATK,  0x02,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x80,    0,   4,           0,   460, 0x80,   0x6a,  ATK,  0x03,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x80,    0,   4,           0,   460, 0x80,   0x6a,  ATK,  0x03,       0x16,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x80,    0,   6,           0,   461, 0x80,   0x6a,  ATK,  0x04,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x80,    0,   2,           0,   462, 0x80,   0x6a,  ATK,  0x05,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x81,    0,  10,           0,   463, 0x80,   0x6a,  ATK,  0x06,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   5,           0,   464, 0x80,   0x6a,  ATK,  0x07,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,  11, END_OF_DATA,   465, 0x80,   0x47,  ATK,  0x08,       0x14,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl17_super_00 ),
};

/* Char_Super 1 */
const Sint32 pl17_super_01[] = {
	CG_FRAME(    0,    0,   1,           0,   466, 0x80,   0x47,  ATK,  0x09,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   465, 0x80,   0x47,  ATK,  0x0a,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl17_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl17_super_02[] = {
	CG_FRAME(    0,    0,   5,           0,   467, 0x80,   0x4a,  ATE,  0x0b,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,  48,           0,   468, 0x80,   0x4a,  ATE,  0x0c,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   5,           0,   469, 0x80,   0x0d,  ATE,  0x80,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   277, 0x80,   0x0d,  ATE,  0x80,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl17_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl17_super_03[] = {
	CG_FRAME(    0,    0,   3,           0,   481, 0x80,   0x11,  ATK,  0x01,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    1,   3,           0,   482, 0x80,   0x11,  ATK,  0x02,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    0,   8,           0,   483, 0x80,   0x11,  ATK,  0x03,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    0,   5,           0,   484, 0x80,   0x11,  ATK,  0x04,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    0,   2,           0,   485, 0x80,   0x11,  ATK,  0x05,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    0,   8,           0,   486, 0x80,   0x11,  ATK,  0x06,       0x16,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x81,    0,   3,           0,   487, 0x80,   0x0b,  ATK,  0x07,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   488, 0x80,   0x05,  ATK,  0x08,       0x14,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl17_super_00 ),
};

/* Char_Super 4 */
const Sint32 pl17_super_04[] = {
	CG_FRAME(    0,    0,   1,           0,   489, 0x80,   0x05,  ATK,  0x09,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   488, 0x80,   0x05,  ATK,  0x0a,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl17_super_04 ),
};

/* Char_Super 5 */
const Sint32 pl17_super_05[] = {
	CG_FRAME(    0,    0,   2,           0,   489, 0x80,   0x05,  ATE,  0x09,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   3,           0,   488, 0x80,   0x05,  ATE,  0x0a,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   6,           0,   490, 0x80,   0x09,  ATE,  0x0b,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x09,  ATE,  0x0c,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   5,           0,   492, 0x80,   0x0d,  ATE,  0x80,          0,    0,   0,     0,      0,  92 ),
};

const Sint32 pl17_super_05_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   125, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   164, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_super_05_loop ),
};

/* Char_Super 6: timeflip_init (Time Flip (hyper)) */
const Sint32 pl17_super_06[] = {
	CG_FRAME(    0,    0,   5,           0,   493, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   9,           0,   494, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   5,           0,   495, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   5,           0,   496, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   5,           0,   497, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   5,           0,   498, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    1,   4,           0,   495, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,   4,           0,   496, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,   4,           0,   497, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,   4,           0,   498, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,   4,           0,   495, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,   4,           0,   496, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,   4,           0,   497, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x81,    2,   4,           0,   498, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
};

const Sint32 pl17_super_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   495, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   4,           0,   496, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   4,           0,   497, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   498, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl17_super_06_loop ),
};

/* Char_Super 7 */
const Sint32 pl17_super_07[] = {
	CG_FRAME(    0,    0,   3,           0,   493, 0x80,   0x4c,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   230, 0x80,   0x11,  ATE,  0x00,       0x05,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   236, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   236, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   230, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl17_super_07 ),
};

/* Char_Effect 0: shl17_00_init */
const Sint32 pl17_effect_00[] = {
	CG_FRAME(    0,    0,   1,           0, 32775, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   1,           0, 32774, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   1,           0, 32773, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32772, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl17_effect_00 ),
};

/* Char_Effect 5: shl17_03_init */
const Sint32 pl17_effect_05[] = {
	CG_FRAME(    0,    0,   1,           0, 32782, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0, 32783, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0, 32784, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32785, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl17_effect_05 ),
};

/* Char_Effect 1: shl17_00_init */
const Sint32 pl17_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0, 32780, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32781, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl17_effect_01 ),
};

/* Char_Effect 6: shl17_03_init */
const Sint32 pl17_effect_06[] = {
	CG_FRAME(    0,    0,   2,           0, 32806, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32807, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl17_effect_06 ),
};

/* Char_Effect 2: shl17_00_init */
const Sint32 pl17_effect_02[] = {
	CG_FRAME(    0,    0,   1,           0, 32779, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   1,           0, 32778, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   1,           0, 32777, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32776, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl17_effect_02 ),
};

/* Char_Effect 7: shl17_03_init */
const Sint32 pl17_effect_07[] = {
	CG_FRAME(    0,    0,   1,           0, 32794, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32797, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl17_effect_07 ),
};

/* Char_Effect 10: shl17_02_init */
const Sint32 pl17_effect_10[] = {
	CG_FRAME(    0,    2,   2,           0,   431, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2,           0,   432, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2,           0,   433, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   433, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl17_effect_10 ),
};

/* Char_Effect 13: shl17_01_init */
const Sint32 pl17_effect_13[] = {
	CG_FRAME(    0,    0,   3,           0,   548, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   3,           0,   550, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   3,           0,   551, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   552, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl17_effect_13 ),
};

/* Char_Effect 14: shl17_01_move */
const Sint32 pl17_effect_14[] = {
	CG_FRAME(    0,    0,   3,           0,   553, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x002a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   554, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x002a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   555, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x002a,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   556, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x002a,   0 ),
	CG_NEXT( pl17_effect_14 ),
};

/* Char_Catch 2
   Char_Effect 15: shl17_01_burst */
const Sint32 pl17_catch_02[] = {
	CG_FRAME(    0,    0,   1,           0, 32768, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x002a,   0 ),
};

const Sint32 pl17_catch_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32768, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x002a, 101 ),
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x11,  ATK,  0x00, 0x4f | CSE,    0,   0,     0, 0x002a, 101 ),
	CG_FRAME(    0,    0,   2,           0, 32768, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x002c, 101 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32769, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0, 0x002c, 101 ),
	CG_NEXT( pl17_catch_02_loop ),
};

/* Char_Effect 3: pls17_00_init */
const Sint32 pl17_effect_03[] = {
	CG_FRAME(    0,    0,   1,           0,   569, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   565, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   570, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   566, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   571, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   567, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   572, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   568, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_effect_03 ),
};

/* Char_Effect 8: pls17_01_init */
const Sint32 pl17_effect_08[] = {
	CG_FRAME(    0,    1,   1,           0, 32786, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32787, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0, 32788, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32792, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32789, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32793, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_effect_08 ),
};

/* Char_Effect 4: pls17_00_init */
const Sint32 pl17_effect_04[] = {
	CG_FRAME(    0,    0,   1,           0,   581, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   577, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   582, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   578, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   583, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   579, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   584, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   580, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_effect_04 ),
};

/* Char_Effect 9: pls17_01_init */
const Sint32 pl17_effect_09[] = {
	CG_FRAME(    0,    1,   1,           0, 32798, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32802, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32799, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32803, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0, 32800, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32804, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32801, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32805, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_effect_09 ),
};

/* Char_Effect 11: shl17_04_init, shl17_04_show_init */
const Sint32 pl17_effect_11[] = {
	CG_FRAME(    0,    0,   1,           0,   662, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   1,           0,   663, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   1,           0,   664, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   665, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl17_effect_11 ),
};

/* Char_Effect 12: shl17_05_fly */
const Sint32 pl17_effect_12[] = {
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME( 0x80,    0,   2,           0,   257, 0x80,   0x11,  ATK,  0x41,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    0,   2,           0,   298, 0x80,   0x11,  ATK,  0x41,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   274, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME( 0x80,    0,   1,           0,   275, 0x80,   0x11,  ATK,  0x41,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   4,           0,   276, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0,   279, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x80,   0x11,  ATK,  0x41,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   2,           0,   283, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   2,           0,   284, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x80,   0x11,  ATK,  0x41,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0x11,  ATK,  0x41,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,   4,           0,   318, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    0,   2,           0,   319, 0x80,   0x11,  ATK,  0x41,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   4,           0,   320, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   3,           0,   323, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   1,           0,   324, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   1,           0,   325, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   5,           0,   326, 0x80,   0x11,  ATK,  0x41,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   2,           0,   327, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   2,           0,   328, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   1,           0,   330, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   285, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   6,           0,   288, 0x80,   0x11,  ATK,  0x41,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   2,           0,   289, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0,   290, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0,   291, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0,   261, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0,   262, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   264, 0x80,   0x0a,  ATK,  0x41,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    2,   5,           0,   265, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0, 0x0100, 104 ),
	CG_FRAME(    0,    3,   4,           0,   266, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0, 0x0200, 104 ),
	CG_FRAME(    0,    0,   4,           0,   267, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0, 0x0300, 104 ),
	CG_FRAME(    0,    0,   5,           0,   268, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0,   265, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,   15,   4,           0,   265, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0, 0x0100, 104 ),
	CG_FRAME(    0,    3,   8,           0,   266, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0, 0x0200, 104 ),
	CG_FRAME(    0,    0,   8,           0,   267, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0, 0x0300, 104 ),
	CG_FRAME(    0,    0,   6,           0,   268, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   5,           0,   265, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   5,           0,   269, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   270, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_NEXT( pl17_effect_12 ),
};

/* Char_Effect 19 */
const Sint32 pl17_effect_19[] = {
	CG_FRAME(    0,    0,   1,           0,   667, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   668, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   669, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   670, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   671, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   672, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   673, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   674, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_effect_19 ),
};

/* Char_Effect 18: shl17_05_init */
const Sint32 pl17_effect_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   115, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl17_effect_18 ),
};

/* Char_Effect 16 */
const Sint32 pl17_effect_16[] = {
	CG_FRAME(    0,    0,   1,           0,   667, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   668, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   669, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   670, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   671, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   672, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   673, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   674, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl17_effect_17[] = {
	CG_FRAME(    0,    0,   1,           0,   667, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   668, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   669, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   670, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   671, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   672, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   673, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   674, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_effect_17 ),
};

/* Char_Effect 20: shl17_06_hit_init */
const Sint32 pl17_effect_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   548, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl17_effect_20 ),
};

/* Char_Catch 0 */
const Sint32 pl17_catch_00[] = {
	CG_FRAME(    0,    0,   6,           0,   362, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,   4,           0,   363, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x80,   0x11,    0,  0x1e, 0x2c | CSE,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,   6,           0,   365, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,   1,           0,   366, 0x80,   0x11,    0,  0x10,          0,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    1,   3,           0,   366, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   367, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   368, 0x80,   0x11,    0,  0x30,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   369, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   370, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   371, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   372, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   373, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_catch_00_loop[] = {
	CG_FRAME(    1,    0,   1, END_OF_DATA,   373, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_catch_00_loop ),
};

/* Char_Catch 3 */
const Sint32 pl17_catch_03[] = {
	CG_FRAME(    0,    0,   6,           0,   362, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,   4,           0,   363, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,   6,           0,   365, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,   1,           0,   366, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    1,   3,           0,   366, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   368, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   369, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_catch_03_loop[] = {
	CG_FRAME(    1,    0,   1, END_OF_DATA,   369, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_catch_03_loop ),
};

/* Char_Catch 1: psycharge_set */
const Sint32 pl17_catch_01[] = {
	CG_FRAME(    0,    0,  18,           0,   375, 0x80,   0x11,    0,  0x00, 0x4f | CSE,    0,   0,     0, 0x0011,   1 ),
	CG_FRAME(    0,   11,   5,           0,   374, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0f12,   1 ),
	CG_FRAME(    0,    0,   5,           0,   375, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0013,   1 ),
	CG_FRAME(    0,    8,   5,           0,   374, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,   5,           0,   375, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0013,   1 ),
	CG_FRAME(    0,    8,   5,           0,   374, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,   5,           0,   375, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0013,   1 ),
	CG_FRAME(    0,    8,   5,           0,   374, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,   5,           0,   375, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0013,   1 ),
	CG_FRAME(    0,    0,   8,           0,   376, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x001a,   1 ),
	CG_FRAME(    0,    0,  10,           0,   377, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x001b,   1 ),
	CG_FRAME(    0,    2,   5,           0,   378, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x001c,   1 ),
	CG_FRAME(    0,    1,  10,           0,   379, 0x80,   0x11,    0,  0x00, 0x49 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   380, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   381, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   382, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_catch_01_loop[] = {
	CG_FRAME(    0,    0,   8,           0,   383, 0x80,   0x11,    0,  0x01, 0x7f | CSE,    0,   0,     0, 0x1000,   1 ),
	CG_FRAME(    0,    4,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01, 0x4f | CSE,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0400,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0500,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0700,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0800,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0900,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0b00,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0c00,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0d00,   1 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x0e00,   1 ),
	CG_FRAME(    0,    0,  32, END_OF_DATA,   383, 0x80,   0x11,    0,  0x01,          0,    0,   0,     0, 0x1000,   1 ),
	CG_NEXT( pl17_catch_01_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl17_change_00[] = {
	CG_FRAME(    0,    0,   1,           0,   176, 0x80,   0x00,    0,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
};

const Sint32 pl17_change_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  10, END_OF_DATA,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl17_change_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   175, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl17_change_02[] = {
	CG_FRAME(    0,    0,   6,           0,   126, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl17_change_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   125, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl17_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl17_change_03[] = {
	CG_FRAME(    0,    0,   1,           0,   183, 0x80,   0x00,    0,  0x00,       0x0e,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_change_03_loop[] = {
	CG_FRAME(    0,    0,   7,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  15,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl17_change_04[] = {
	CG_FRAME(    0,    0,   2,           0,   202, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   202, 0x80,   0x13,    0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   204, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   205, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   204, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x54,    0,  0x00, 0x18 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0x54,    0,  0x00, 0x18 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   209, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   210, 0x80,   0x54,    0,  0x00, 0x18 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   211, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   202, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_change_04 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl17_extra_00[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   359, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl17_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl17_extra_01[] = {
	CG_FRAME(    0,    0,   1,           0,   180, 0x80,   0x53,  ATE,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_extra_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   180, 0x80,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   181, 0x80,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   182, 0x80,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   181, 0x80,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   182, 0x80,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   181, 0x80,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   182, 0x80,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   181, 0x80,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   180, 0x80,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_extra_01_loop ),
};

/* Char_Extra 2: chouhatsu_init (the taunt), fly_start5 */
const Sint32 pl17_extra_02[] = {
	CG_FRAME(    0,    0,   1,           0,   202, 0x00,   0x13,    0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_extra_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   202, 0x00,   0x13,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   203, 0x00,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x00,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   205, 0x00,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   204, 0x00,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_extra_02_loop ),
};

/* Char_Extra 3: chouhatsu_move (the taunt), chouhatsu_loop (the taunt), fly_start5 */
const Sint32 pl17_extra_03[] = {
	CG_FRAME(    0,    0,   3,           0,   203, 0x00,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x00,   0x54,    0,  0x00, 0x18 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x00,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x00,   0x54,    0,  0x00, 0x18 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   209, 0x00,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   210, 0x00,   0x54,    0,  0x00, 0x18 | CSE,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_extra_03 ),
};

/* Char_Extra 4: chouhatsu_loop (the taunt) */
const Sint32 pl17_extra_04[] = {
	CG_FRAME(    0,    0,   4,           0,   203, 0x00,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  14,           0,   211, 0x00,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   202, 0x00,   0x13,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_extra_04 ),
};

/* Char_Extra 5: pl17_upper */
const Sint32 pl17_extra_05[] = {
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   257, 0x80,   0x0d,  ATK,  0x00,       0x03,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   6,           0,   258, 0x80,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x0d,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   260, 0x80,   0x0d,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl17_extra_05 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl17_dizzy_00[] = {
	CG_FRAME(    0,    0,   4,           0,   503, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   504, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   505, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   504, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_dizzy_00 ),
};

/* Char_Win 0: win00_init */
const Sint32 pl17_win_00[] = {
	CG_FRAME(    0,    0,   2,           0,   202, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   204, 0x80,   0x54,    0,  0x00, 0x18 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   205, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   204, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x54,    0,  0x00, 0x18 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   209, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   210, 0x80,   0x54,    0,  0x00, 0x18 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  14,           0,   211, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   228, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_win_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   229, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_win_00_loop ),
};

/* Char_Win 1: win01_init */
const Sint32 pl17_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   233, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   234, 0x80,   0x11,    0,  0x00,       0x05,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   235, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   234, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   242, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x11,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   247, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_win_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   250, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   249, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   248, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   247, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_win_01_loop ),
};

/* Char_Win 2: win02_init */
const Sint32 pl17_win_02[] = {
	CG_FRAME(    0,    0,   8,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl17_win_02_loop[] = {
	CG_FRAME(    0,    0,   8,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl17_win_02_loop ),
};

/* Char_Appear 0: app00_init */
const Sint32 pl17_appear_00[] = {
	CG_FRAME(    0,    0,   8,           0,   189, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x00,   0x00,    0,  0x00,       0x1f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   189, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   195, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   197, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   198, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   199, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   200, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   195, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   151, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_appear_00 ),
};

/* Char_Appear 1: app01_init */
const Sint32 pl17_appear_01[] = {
	CG_FRAME(    0,    3,   1,           0,   212, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   3,           0,   212, 0x00,   0x00,    0,  0x00,       0x1e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   213, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   214, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   428, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   429, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   430, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   215, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   217, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   219, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   216, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   217, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   218, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   219, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   216, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   220, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   221, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   222, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   223, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   224, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   225, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   226, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   227, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   217, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   218, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   219, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   216, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   217, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   218, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   216, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   217, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   218, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   219, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   216, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   217, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   218, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   219, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   216, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   217, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   218, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   219, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   2,           0,   126, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl17_appear_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   129, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl17_appear_01_loop ),
};

/* group 27 2: pls17_03_init0 */
const Sint32 pl17_grp27_02[] = {
	EF_FRAME(    0,    0,   5,           0,   613 ),
	EF_FRAME(    0,    0,   5,           0,   614 ),
	EF_FRAME(    0,    0,   5,           0,   615 ),
	EF_FRAME(    0,    0,   5,           0,   616 ),
	EF_FRAME(    0,    0,   5,           0,   617 ),
	EF_FRAME(    0,    0,   5,           0,   618 ),
	EF_FRAME(    0,    0,   5,           0,   619 ),
	EF_FRAME(    0,    0,   5,           0,   620 ),
	EF_FRAME(    0,    0,   8,           0,   621 ),
	EF_FRAME(    0,    2,  10, END_OF_DATA,   622 ),
	CG_NEXT( pl17_grp27_02 ),
};

/* group 27 3: pls17_03_init3 */
const Sint32 pl17_grp27_03[] = {
	EF_FRAME(    0,    0,   2,           0,   629 ),
	EF_FRAME(    0,    0,   2,           0,   630 ),
	EF_FRAME(    0,    0,   2,           0,   631 ),
	EF_FRAME(    0,    0,   2,           0,   632 ),
	EF_FRAME(    0,    0,   2,           0,   633 ),
	EF_FRAME(    0,    0,   2,           0,   634 ),
	EF_FRAME(    0,    0,   2,           0,   635 ),
	EF_FRAME(    0,    0,   2,           0,   636 ),
	EF_FRAME(    0,    0,   2,           0,   623 ),
	EF_FRAME(    0,    0,   2,           0,   624 ),
	EF_FRAME(    0,    0,   2,           0,   625 ),
	EF_FRAME(    0,    0,   2,           0,   626 ),
	EF_FRAME(    0,    0,   2,           0,   627 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   628 ),
	CG_NEXT( pl17_grp27_03 ),
};

/* group 27 4: pls17_03_init1 */
const Sint32 pl17_grp27_04[] = {
	EF_FRAME(    0,    0,   2,           0,   637 ),
	EF_FRAME(    0,    0,   2,           0,   638 ),
	EF_FRAME(    0,    0,   2,           0,   639 ),
	EF_FRAME(    0,    0,   2,           0,   640 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   641 ),
	CG_NEXT( pl17_grp27_04 ),
};

/* group 27 5: pls17_03_init2 */
const Sint32 pl17_grp27_05[] = {
	EF_FRAME(    0,    0,   2,           0,   642 ),
	EF_FRAME(    0,    0,   2,           0,   643 ),
	EF_FRAME(    0,    0,   2,           0,   644 ),
	EF_FRAME(    0,    0,   2,           0,   645 ),
	EF_FRAME(    0,    0,   2,           0,   646 ),
	EF_FRAME(    0,    0,   2,           0,   647 ),
	EF_FRAME(    0,    0,   2,           0,   648 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   649 ),
	CG_NEXT( pl17_grp27_05 ),
};

/* group 27 6: pls17_04_init0 */
const Sint32 pl17_grp27_06[] = {
	EF_FRAME(    0,    3,   2,           0,   212 ),
	EF_FRAME(    0,    3,   2,           0,   213 ),
	EF_FRAME(    0,    3,   2,           0,   214 ),
	EF_FRAME(    0,    3,   2,           0,   428 ),
	EF_FRAME(    0,    3,   2,           0,   429 ),
	EF_FRAME(    0,    3,   2,           0,   430 ),
	EF_FRAME(    0,    3,   2,           0,   215 ),
	EF_FRAME(    0,    3,   2,           0,   217 ),
	EF_FRAME(    0,    3,   2,           0,   219 ),
	EF_FRAME(    0,    3,   2,           0,   216 ),
	EF_FRAME(    0,    3,   2,           0,   217 ),
	EF_FRAME(    0,    3,   2,           0,   218 ),
	EF_FRAME(    0,    3,   2,           0,   219 ),
	EF_FRAME(    0,    3,   2,           0,   216 ),
	EF_FRAME(    0,    3,   2,           0,   220 ),
	EF_FRAME(    0,    3,   2,           0,   221 ),
	EF_FRAME(    0,    3,   2,           0,   222 ),
	EF_FRAME(    0,    3,   2,           0,   223 ),
	EF_FRAME(    0,    3,   2,           0,   224 ),
	EF_FRAME(    0,    3,   2,           0,   225 ),
	EF_FRAME(    0,    3,   2,           0,   226 ),
	EF_FRAME(    0,    3,   2,           0,   227 ),
	EF_FRAME(    0,    3,   2, END_OF_DATA,   217 ),
	CG_NEXT( pl17_grp27_06 ),
};

/* group 27 0: pls17_05_init0 */
const Sint32 pl17_grp27_00[] = {
	EF_FRAME(    0,    0,   1,           0,   650 ),
	EF_FRAME(    0,    0,   1,           0,   651 ),
	EF_FRAME(    0,    0,   1,           0,   652 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   653 ),
	CG_NEXT( pl17_grp27_00 ),
};

/* group 27 1: pls17_05_init1, pls17_05_move1 */
const Sint32 pl17_grp27_01[] = {
	EF_FRAME(    0,    0,   4,           0,   654 ),
	EF_FRAME(    0,    0,   4,           0,   655 ),
	EF_FRAME(    0,    0,   5,           0,   656 ),
	EF_FRAME(    0,    0,   5,           0,   657 ),
	EF_FRAME(    0,    2,   4,           0,   658 ),
	EF_FRAME(    0,    2,   4,           0,   659 ),
	EF_FRAME(    0,    2,   4,           0,   660 ),
	EF_FRAME(    0,    3,   4, END_OF_DATA,   661 ),
	CG_NEXT( pl17_grp27_01 ),
};

/* group 27 7: pls17_04_init1 */
const Sint32 pl17_grp27_07[] = {
	EF_FRAME(    0,    3,   4, END_OF_DATA,   666 ),
	CG_NEXT( pl17_grp27_07 ),
};

/* group 27 8: pls17_04_init2 */
const Sint32 pl17_grp27_08[] = {
	EF_FRAME(    0,    0,   6,           0,   189 ),
	EF_FRAME(    0,    2,   4,           0,   190 ),
	EF_FRAME(    0,    0,   4,           0,   191 ),
	EF_FRAME(    0,    0,   4,           0,   192 ),
	EF_FRAME(    0,    0,   6,           0,   189 ),
	EF_FRAME(    0,    0,   4,           0,   193 ),
	EF_FRAME(    0,    0,   4,           0,   194 ),
	EF_FRAME(    0,    0,   6,           0,   195 ),
	EF_FRAME(    0,    0,   4,           0,   196 ),
	EF_FRAME(    0,    0,   4,           0,   197 ),
	EF_FRAME(    0,    0,   4,           0,   198 ),
	EF_FRAME(    0,    0,   4,           0,   199 ),
	EF_FRAME(    0,    0,   4,           0,   200 ),
	EF_FRAME(    0,    0,   4,           0,   201 ),
	EF_FRAME(    0,    0,   6,           0,   195 ),
	EF_FRAME(    0,    0,   4,           0,   149 ),
	EF_FRAME(    0,    0,   4,           0,   150 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   151 ),
	CG_NEXT( pl17_grp27_08 ),
};

/* group 27 9: pls17_04_init3 */
const Sint32 pl17_grp27_09[] = {
	EF_FRAME(    0,    0,   4,           0,   186 ),
	EF_FRAME(    0,    0,   4,           0,   131 ),
	EF_FRAME(    0,    0,   4,           0,   187 ),
	EF_FRAME(    0,    0,   4,           0,   188 ),
	EF_FRAME(    0,    0,   4,           0,   187 ),
	EF_FRAME(    0,    0,   4,           0,   188 ),
	EF_FRAME(    0,    0,   4,           0,   187 ),
	EF_FRAME(    0,    0,   4,           0,   188 ),
	EF_FRAME(    0,    0,   4,           0,   187 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   188 ),
	CG_NEXT( pl17_grp27_09 ),
};

/* group 27 10: pls17_06_init1 */
const Sint32 pl17_grp27_10[] = {
	EF_FRAME(    0,    0,   4, END_OF_DATA,   273 ),
	CG_NEXT( pl17_grp27_10 ),
};

/* group 27 11: pls17_06_init0 */
const Sint32 pl17_grp27_11[] = {
	EF_FRAME(    0,    0,  10,           0,   675 ),
	EF_FRAME(    0,    0,   5, END_OF_DATA,   676 ),
	CG_NEXT( pl17_grp27_11 ),
};

/* group 27 12: pls17_06_init0 */
const Sint32 pl17_grp27_12[] = {
	EF_FRAME(    0,    0,  10,           0,   677 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   678 ),
	CG_NEXT( pl17_grp27_12 ),
};

/* group 27 13: pls17_06_init0 */
const Sint32 pl17_grp27_13[] = {
	EF_FRAME(    0,    0,  10,           0,   679 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   680 ),
	CG_NEXT( pl17_grp27_13 ),
};

/* group 27 14: shl17_01_spark_init */
const Sint32 pl17_grp27_14[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA, 32770 ),
	CG_NEXT( pl17_grp27_14 ),
};

/* group 27 15: shl17_01_spark_init */
const Sint32 pl17_grp27_15[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA, 32771 ),
	CG_NEXT( pl17_grp27_15 ),
};

/* group 27 16: shl17_01_flash_init */
const Sint32 pl17_grp27_16[] = {
	EF_FRAME(    0,    0,   1,           0, 32768 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA, 32769 ),
	CG_NEXT( pl17_grp27_16 ),
};

/* group 27 17: pls17_00_init2 */
const Sint32 pl17_grp27_17[] = {
	EF_FRAME(    0,    0,   4,           0,   416 ),
	EF_FRAME(    0,    0,   4,           0,   417 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   418 ),
};

/* group 27 18 */
const Sint32 pl17_grp27_18[] = {
	EF_FRAME(    0,    0,   1,           0,   419 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   420 ),
};

/* group 27 19 */
const Sint32 pl17_grp27_19[] = {
	EF_FRAME(    0,    0,   5,           0,   418 ),
	EF_FRAME(    0,    0,   5,           0,   417 ),
	EF_FRAME(    0,    0,   5, END_OF_DATA,   416 ),
	CG_NEXT( pl17_grp27_19 ),
};

/* group 27 20: pls17_00_init2 */
const Sint32 pl17_grp27_20[] = {
	EF_FRAME(    0,    0,   4,           0,   452 ),
	EF_FRAME(    0,    0,   4,           0,   453 ),
	EF_FRAME(    0,    0,  10, END_OF_DATA,   454 ),
};

/* group 27 21 */
const Sint32 pl17_grp27_21[] = {
	EF_FRAME(    0,    0,   1,           0,   455 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   456 ),
};

/* group 27 22 */
const Sint32 pl17_grp27_22[] = {
	EF_FRAME(    0,    0,   5,           0,   454 ),
	EF_FRAME(    0,    0,   5,           0,   453 ),
	EF_FRAME(    0,    0,   5, END_OF_DATA,   452 ),
	CG_NEXT( pl17_grp27_22 ),
};

/* group 27 23 */
const Sint32 pl17_grp27_23[] = {
	EF_FRAME(    0,    1,   4,           0,   470 ),
	EF_FRAME(    0,    1,   4,           0,   471 ),
	EF_FRAME(    0,    1,   4,           0,   472 ),
	EF_FRAME(    0,    1,  14,           0,   473 ),
	EF_FRAME(    0,    1,   6,           0,   474 ),
	EF_FRAME(    0,    1,   2,           0,   475 ),
	EF_FRAME(    0,    1,  14,           0,   476 ),
	EF_FRAME(    0,    1,   4,           0,   477 ),
};

/* group 27 24 */
const Sint32 pl17_grp27_24[] = {
	EF_FRAME(    0,    0,   1,           0,   478 ),
	EF_FRAME(    0,    0,   1,           0,   477 ),
};

/* group 27 25 */
const Sint32 pl17_grp27_25[] = {
	EF_FRAME(    0,    1,   3,           0,   479 ),
	EF_FRAME(    0,    1,   7, END_OF_DATA,   480 ),
	CG_NEXT( pl17_grp27_25 ),
};

/* group 27 26 */
const Sint32 pl17_grp27_26[] = {
	EF_FRAME(    0,    0,   4,           0,   470 ),
	EF_FRAME(    0,    0,   4,           0,   471 ),
	EF_FRAME(    0,    0,   8,           0,   472 ),
	EF_FRAME(    0,    0,   6,           0,   473 ),
	EF_FRAME(    0,    0,   2,           0,   474 ),
	EF_FRAME(    0,    0,   8,           0,   475 ),
	EF_FRAME(    0,    0,   4,           0,   476 ),
	EF_FRAME(    0,    0,   8,           0,   477 ),
};

/* group 27 27 */
const Sint32 pl17_grp27_27[] = {
	EF_FRAME(    0,    0,   1,           0,   478 ),
	EF_FRAME(    0,    0,   1,           0,   477 ),
};

/* group 27 28 */
const Sint32 pl17_grp27_28[] = {
	EF_FRAME(    0,    0,   3,           0,   479 ),
	EF_FRAME(    0,    0,   5, END_OF_DATA,   480 ),
	CG_NEXT( pl17_grp27_28 ),
};
