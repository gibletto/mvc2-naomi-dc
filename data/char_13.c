/* Ruby Heart (character 13): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_13      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_13     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl13.c) says which group
                   and number each move uses.
   pl13_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl13_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl13_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl13_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_13
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_13, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_13 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl13_special_00[], pl13_special_01[], pl13_special_02[], pl13_special_02_loop[],
	pl13_special_03[], pl13_special_03_loop[], pl13_special_04[], pl13_special_04_loop[],
	pl13_special_05[], pl13_special_05_loop[], pl13_special_06[], pl13_special_06_loop[],
	pl13_special_07[], pl13_special_07_loop[], pl13_special_08[], pl13_special_32[],
	pl13_super_17_loop[], pl13_super_00[], pl13_super_00_loop[], pl13_super_01[], pl13_super_02[],
	pl13_super_02_loop[], pl13_super_03[], pl13_super_03_loop[], pl13_super_04[],
	pl13_super_04_loop[], pl13_super_05[], pl13_super_05_loop[], pl13_super_06[],
	pl13_super_06_loop[], pl13_super_07[], pl13_super_07_loop[], pl13_super_08[], pl13_super_17[],
	pl13_special_09[], pl13_special_09_loop[], pl13_super_09[], pl13_super_09_loop[],
	pl13_super_14[], pl13_super_16[], pl13_special_10[], pl13_special_10_loop[], pl13_special_11[],
	pl13_special_12[], pl13_special_12_loop[], pl13_special_13[], pl13_special_13_loop[],
	pl13_special_14[], pl13_special_15[], pl13_special_25[], pl13_special_24[], pl13_special_23[],
	pl13_special_22[], pl13_special_21[], pl13_special_20[], pl13_special_19[], pl13_special_18[],
	pl13_special_17[], pl13_special_16[], pl13_special_26[], pl13_special_27[], pl13_special_28[],
	pl13_special_29[], pl13_special_30[], pl13_super_10[], pl13_super_10_loop[], pl13_super_11[],
	pl13_super_11_loop[], pl13_super_12[], pl13_super_12_loop[], pl13_super_13[],
	pl13_super_13_loop[], pl13_special_31[], pl13_special_31_loop[], pl13_extra_04[],
	pl13_extra_04_loop[], pl13_stand_00[], pl13_stand_02[], pl13_stand_03[], pl13_jump_00[],
	pl13_jump_00_loop[], pl13_jump_01[], pl13_jump_01_loop[], pl13_jump_02[], pl13_jump_02_loop[],
	pl13_jump_03[], pl13_jump_06[], pl13_jump_06_loop[], pl13_jump_09[], pl13_jump_09_loop[],
	pl13_dash_00[], pl13_dash_00_loop[], pl13_dash_02[], pl13_dash_01[], pl13_dash_01_loop[],
	pl13_dash_03[], pl13_crouch_00[], pl13_crouch_01[], pl13_crouch_02[], pl13_turn_00[],
	pl13_turn_01[], pl13_guard_00[], pl13_guard_01[], pl13_guard_02[], pl13_guard_04[],
	pl13_guard_05[], pl13_guard_06[], pl13_air_guard_00[], pl13_air_guard_01[], pl13_unused_00[],
	pl13_stand_p_00[], pl13_stand_p_01[], pl13_stand_p_02[], pl13_stand_k_00[], pl13_stand_k_01[],
	pl13_stand_k_02[], pl13_crouch_p_00[], pl13_crouch_p_01[], pl13_crouch_p_02[],
	pl13_crouch_k_00[], pl13_crouch_k_01[], pl13_crouch_k_02[], pl13_jump_p_00[], pl13_jump_p_01[],
	pl13_jump_p_02[], pl13_jump_k_00[], pl13_jump_k_00_loop[], pl13_jump_k_01[],
	pl13_jump_k_01_loop[], pl13_jump_k_02[], pl13_jump_k_02_loop[], pl13_extra_03[], pl13_catch_00[],
	pl13_catch_01[], pl13_get_up_00[], pl13_get_up_01[], pl13_get_up_02[], pl13_get_up_03[],
	pl13_damage_02[], pl13_damage_01[], pl13_damage_00[], pl13_damage_05[], pl13_damage_04[],
	pl13_damage_03[], pl13_damage_08[], pl13_damage_07[], pl13_damage_06[], pl13_damage_09[],
	pl13_damage_09_loop[], pl13_damage_34[], pl13_damage_13[], pl13_damage_13_loop[],
	pl13_damage_26[], pl13_damage_31[], pl13_damage_10[], pl13_damage_10_loop[], pl13_damage_11[],
	pl13_damage_11_loop[], pl13_damage_12[], pl13_damage_12_loop[], pl13_damage_16[],
	pl13_damage_17[], pl13_damage_18[], pl13_damage_19[], pl13_damage_20[], pl13_damage_21[],
	pl13_damage_22[], pl13_damage_23[], pl13_damage_24[], pl13_damage_25[], pl13_damage_27[],
	pl13_damage_33[], pl13_damage_28[], pl13_damage_29[], pl13_caught_00[], pl13_caught_01[],
	pl13_caught_02[], pl13_caught_03[], pl13_caught_04[], pl13_caught_05[], pl13_caught_06[],
	pl13_caught_07[], pl13_caught_08[], pl13_caught_09[], pl13_caught_10[], pl13_caught_11[],
	pl13_caught_12[], pl13_caught_13[], pl13_caught_14[], pl13_caught_15[], pl13_caught_16[],
	pl13_caught_17[], pl13_caught_18[], pl13_caught_19[], pl13_caught_20[], pl13_caught_21[],
	pl13_dizzy_00[], pl13_win_00[], pl13_win_00_loop[], pl13_win_01[], pl13_win_01_loop[],
	pl13_unused_01[], pl13_win_02[], pl13_change_00[], pl13_change_00_loop[], pl13_change_01[],
	pl13_change_02[], pl13_change_02_loop[], pl13_change_03[], pl13_change_03_loop[],
	pl13_appear_00[], pl13_extra_00[], pl13_extra_00_loop[], pl13_extra_01[], pl13_extra_01_loop[],
	pl13_extra_02[], pl13_extra_02_loop[], pl13_effect_00[], pl13_effect_56[], pl13_effect_58[],
	pl13_effect_59[], pl13_effect_01[], pl13_effect_01_loop[], pl13_effect_57[],
	pl13_effect_57_loop[], pl13_effect_29[], pl13_effect_30[], pl13_effect_02[], pl13_effect_49[],
	pl13_effect_49_loop[], pl13_effect_03[], pl13_effect_60[], pl13_effect_04[], pl13_effect_55[],
	pl13_effect_05[], pl13_effect_06[], pl13_effect_07[], pl13_effect_07_loop[], pl13_effect_08[],
	pl13_effect_09[], pl13_effect_10[], pl13_effect_11[], pl13_effect_12[], pl13_effect_13[],
	pl13_effect_14[], pl13_effect_15[], pl13_effect_16[], pl13_effect_17[], pl13_effect_18[],
	pl13_effect_19[], pl13_effect_20[], pl13_effect_21[], pl13_effect_22[], pl13_effect_23[],
	pl13_effect_47[], pl13_effect_48[], pl13_effect_48_loop[], pl13_effect_44[],
	pl13_effect_44_loop[], pl13_effect_24[], pl13_effect_25[], pl13_effect_26[], pl13_effect_27[],
	pl13_effect_45[], pl13_effect_46[], pl13_effect_31[], pl13_effect_31_loop[], pl13_effect_32[],
	pl13_effect_33[], pl13_effect_34[], pl13_effect_34_loop[], pl13_effect_35[], pl13_effect_36[],
	pl13_catch_02[], pl13_catch_03[], pl13_catch_04[], pl13_catch_05[], pl13_catch_06[],
	pl13_catch_07[], pl13_effect_43[], pl13_effect_50[], pl13_effect_51[], pl13_effect_52[],
	pl13_effect_53[], pl13_effect_54[], pl13_grp27_00[], pl13_grp27_01[], pl13_grp27_04[],
	pl13_grp27_07[], pl13_grp27_08[], pl13_grp27_09[], pl13_grp27_10[], pl13_grp27_11[],
	pl13_grp27_12[], pl13_grp27_02[], pl13_grp27_05[], pl13_grp27_03[], pl13_grp27_06[],
	pl13_grp27_13[], pl13_grp27_13_loop[], pl13_grp27_14[];
extern const Sint32 *const pl13_stand_tbl[], *const pl13_jump_tbl[], *const pl13_dash_tbl[],
	*const pl13_crouch_tbl[], *const pl13_turn_tbl[], *const pl13_guard_tbl[],
	*const pl13_air_guard_tbl[], *const pl13_stand_p_tbl[], *const pl13_stand_k_tbl[],
	*const pl13_crouch_p_tbl[], *const pl13_crouch_k_tbl[], *const pl13_jump_p_tbl[],
	*const pl13_jump_k_tbl[], *const pl13_damage_tbl[], *const pl13_dizzy_tbl[],
	*const pl13_catch_tbl[], *const pl13_caught_tbl[], *const pl13_get_up_tbl[],
	*const pl13_appear_tbl[], *const pl13_win_tbl[], *const pl13_extra_tbl[],
	*const pl13_special_tbl[], *const pl13_super_tbl[], *const pl13_effect_tbl[],
	*const pl13_change_tbl[], *const pl13_grp25_tbl[], *const pl13_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_13[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 8192, -8, -24, 0, 0, 0 },
	{ 1000, 1000, 16384, 16, -48, 0, 0, 0 },
	{ 1000, 1000, -8192, 8, -24, 0, 0, 0 },
	{ 1000, 1000, -16384, -16, -48, 0, 0, 0 },
	{ 1000, 1000, 1024, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -1024, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 2048, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -2048, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_13[] = {
	pl13_stand_tbl, pl13_jump_tbl, pl13_dash_tbl, pl13_crouch_tbl, pl13_turn_tbl, pl13_guard_tbl,
	pl13_air_guard_tbl, pl13_stand_p_tbl, pl13_stand_k_tbl, pl13_crouch_p_tbl, pl13_crouch_k_tbl, pl13_jump_p_tbl,
	pl13_jump_k_tbl, pl13_damage_tbl, pl13_dizzy_tbl, pl13_catch_tbl, pl13_caught_tbl, pl13_get_up_tbl,
	pl13_appear_tbl, pl13_win_tbl, pl13_extra_tbl, pl13_special_tbl, pl13_super_tbl, pl13_effect_tbl,
	pl13_change_tbl, pl13_grp25_tbl, pl13_grp25_tbl, pl13_grp27_tbl, pl13_grp27_tbl, pl13_grp27_tbl,
	pl13_grp27_tbl, pl13_grp27_tbl, pl13_grp27_tbl, 0,
};

const Sint32 *const pl13_stand_tbl[] = {
	pl13_stand_00, pl13_stand_00, pl13_stand_02, pl13_stand_03, pl13_stand_00, pl13_stand_00,
	pl13_stand_02, pl13_stand_03, 0,
};

const Sint32 *const pl13_jump_tbl[] = {
	pl13_jump_00, pl13_jump_01, pl13_jump_02, pl13_jump_03, pl13_jump_03, pl13_jump_03,
	pl13_jump_06, pl13_jump_06, pl13_jump_06, pl13_jump_09, pl13_jump_09, pl13_jump_09,
	0,
};

const Sint32 *const pl13_dash_tbl[] = {
	pl13_dash_00, pl13_dash_01, pl13_dash_02, pl13_dash_03, 0,
};

const Sint32 *const pl13_crouch_tbl[] = {
	pl13_crouch_00, pl13_crouch_01, pl13_crouch_02, 0,
};

const Sint32 *const pl13_turn_tbl[] = {
	pl13_turn_00, pl13_turn_01, 0,
};

const Sint32 *const pl13_guard_tbl[] = {
	pl13_guard_00, pl13_guard_01, pl13_guard_02, pl13_guard_01, pl13_guard_04, pl13_guard_05,
	pl13_guard_06, pl13_guard_05, 0,
};

const Sint32 *const pl13_air_guard_tbl[] = {
	pl13_air_guard_00, pl13_air_guard_01, pl13_air_guard_01, 0,
};

const Sint32 *const pl13_stand_p_tbl[] = {
	pl13_stand_p_00, pl13_stand_p_01, pl13_stand_p_02, 0,
};

const Sint32 *const pl13_stand_k_tbl[] = {
	pl13_stand_k_00, pl13_stand_k_01, pl13_stand_k_02, 0,
};

const Sint32 *const pl13_crouch_p_tbl[] = {
	pl13_crouch_p_00, pl13_crouch_p_01, pl13_crouch_p_02, 0,
};

const Sint32 *const pl13_crouch_k_tbl[] = {
	pl13_crouch_k_00, pl13_crouch_k_01, pl13_crouch_k_02, 0,
};

const Sint32 *const pl13_jump_p_tbl[] = {
	pl13_jump_p_00, pl13_jump_p_01, pl13_jump_p_02, pl13_jump_p_00, pl13_jump_p_01, pl13_jump_p_02,
	0,
};

const Sint32 *const pl13_jump_k_tbl[] = {
	pl13_jump_k_00, pl13_jump_k_01, pl13_jump_k_02, pl13_jump_k_00, pl13_jump_k_01, pl13_jump_k_02,
	0,
};

const Sint32 *const pl13_damage_tbl[] = {
	pl13_damage_00, pl13_damage_01, pl13_damage_02, pl13_damage_03, pl13_damage_04, pl13_damage_05,
	pl13_damage_06, pl13_damage_07, pl13_damage_08, pl13_damage_09, pl13_damage_10, pl13_damage_11,
	pl13_damage_12, pl13_damage_13, pl13_damage_13, pl13_damage_13, pl13_damage_16, pl13_damage_17,
	pl13_damage_18, pl13_damage_19, pl13_damage_20, pl13_damage_21, pl13_damage_22, pl13_damage_23,
	pl13_damage_24, pl13_damage_25, pl13_damage_26, pl13_damage_27, pl13_damage_28, pl13_damage_29,
	pl13_damage_27, pl13_damage_31, pl13_damage_27, pl13_damage_33, pl13_damage_34, 0,
};

const Sint32 *const pl13_dizzy_tbl[] = {
	pl13_dizzy_00, 0,
};

const Sint32 *const pl13_catch_tbl[] = {
	pl13_catch_00, pl13_catch_01, pl13_catch_02, pl13_catch_03, pl13_catch_04, pl13_catch_05,
	pl13_catch_06, pl13_catch_07, 0,
};

const Sint32 *const pl13_caught_tbl[] = {
	pl13_caught_00, pl13_caught_01, pl13_caught_02, pl13_caught_03, pl13_caught_04, pl13_caught_05,
	pl13_caught_06, pl13_caught_07, pl13_caught_08, pl13_caught_09, pl13_caught_10, pl13_caught_11,
	pl13_caught_12, pl13_caught_13, pl13_caught_14, pl13_caught_15, pl13_caught_16, pl13_caught_17,
	pl13_caught_18, pl13_caught_19, pl13_caught_20, pl13_caught_21, 0,
};

const Sint32 *const pl13_get_up_tbl[] = {
	pl13_get_up_00, pl13_get_up_01, pl13_get_up_02, pl13_get_up_03, 0,
};

const Sint32 *const pl13_appear_tbl[] = {
	pl13_appear_00, 0,
};

const Sint32 *const pl13_win_tbl[] = {
	pl13_win_00, pl13_win_01, pl13_win_02, 0,
};

const Sint32 *const pl13_extra_tbl[] = {
	pl13_extra_00, pl13_extra_01, pl13_extra_02, pl13_extra_03, pl13_extra_04, 0,
};

const Sint32 *const pl13_special_tbl[] = {
	pl13_special_00, pl13_special_01, pl13_special_02, pl13_special_03, pl13_special_04, pl13_special_05,
	pl13_special_06, pl13_special_07, pl13_special_08, pl13_special_09, pl13_special_10, pl13_special_11,
	pl13_special_12, pl13_special_13, pl13_special_14, pl13_special_15, pl13_special_16, pl13_special_17,
	pl13_special_18, pl13_special_19, pl13_special_20, pl13_special_21, pl13_special_22, pl13_special_23,
	pl13_special_24, pl13_special_25, pl13_special_26, pl13_special_27, pl13_special_28, pl13_special_29,
	pl13_special_30, pl13_special_31, pl13_special_32, 0,
};

const Sint32 *const pl13_super_tbl[] = {
	pl13_super_00, pl13_super_01, pl13_super_02, pl13_super_03, pl13_super_04, pl13_super_05,
	pl13_super_06, pl13_super_07, pl13_super_08, pl13_super_09, pl13_super_10, pl13_super_11,
	pl13_super_12, pl13_super_13, pl13_super_14, pl13_jump_09, pl13_super_16, pl13_super_17,
	0,
};

const Sint32 *const pl13_effect_tbl[] = {
	pl13_effect_00, pl13_effect_01, pl13_effect_02, pl13_effect_03, pl13_effect_04, pl13_effect_05,
	pl13_effect_06, pl13_effect_07, pl13_effect_08, pl13_effect_09, pl13_effect_10, pl13_effect_11,
	pl13_effect_12, pl13_effect_13, pl13_effect_14, pl13_effect_15, pl13_effect_16, pl13_effect_17,
	pl13_effect_18, pl13_effect_19, pl13_effect_20, pl13_effect_21, pl13_effect_22, pl13_effect_23,
	pl13_effect_24, pl13_effect_25, pl13_effect_26, pl13_effect_27, pl13_effect_00, pl13_effect_29,
	pl13_effect_30, pl13_effect_31, pl13_effect_32, pl13_effect_33, pl13_effect_34, pl13_effect_35,
	pl13_effect_36, pl13_catch_02, pl13_catch_03, pl13_catch_04, pl13_catch_05, pl13_catch_06,
	pl13_catch_07, pl13_effect_43, pl13_effect_44, pl13_effect_45, pl13_effect_46, pl13_effect_47,
	pl13_effect_48, pl13_effect_49, pl13_effect_50, pl13_effect_51, pl13_effect_52, pl13_effect_53,
	pl13_effect_54, pl13_effect_55, pl13_effect_56, pl13_effect_57, pl13_effect_58, pl13_effect_59,
	pl13_effect_60, 0,
};

const Sint32 *const pl13_change_tbl[] = {
	pl13_change_00, pl13_change_01, pl13_change_02, pl13_change_03, pl13_extra_04, 0,
};

const Sint32 *const pl13_grp25_tbl[] = {
	pl13_stand_00, 0,
};

const Sint32 *const pl13_grp27_tbl[] = {
	pl13_grp27_00, pl13_grp27_01, pl13_grp27_02, pl13_grp27_03, pl13_grp27_04, pl13_grp27_05,
	pl13_grp27_06, pl13_grp27_07, pl13_grp27_08, pl13_grp27_09, pl13_grp27_10, pl13_grp27_11,
	pl13_grp27_12, pl13_grp27_13, pl13_grp27_14, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Special 0: schwarz_init (Schwarzaile) */
const Sint32 pl13_special_00[] = {
	CG_FRAME(    0,    0,   3,           0,   403, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   403, 0x80,   0x00,  ATK,  0x00, 0x17,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   404, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   405, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   405, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_special_00 ),
};

/* Char_Special 1: schwarz_next (Schwarzaile) */
const Sint32 pl13_special_01[] = {
	CG_FRAME(    0,    0,   4,           0,   407, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   3,           0,   408, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   409, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl13_special_01 ),
};

/* Char_Special 2: schwarz_fly (Schwarzaile) */
const Sint32 pl13_special_02[] = {
	CG_FRAME(    0,    0,   3,           0,   410, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   411, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   411, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl13_special_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    83, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_special_02_loop ),
};

/* Char_Special 3: schwarz_start (Schwarzaile), schwarz_turn (Schwarzaile, 3 + routine_3) */
const Sint32 pl13_special_03[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    9,   0,     0,      0,  45 ),
};

const Sint32 pl13_special_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    9,   0,     0,      0,  45 ),
	CG_NEXT( pl13_special_03_loop ),
};

/* Char_Special 4 */
const Sint32 pl13_special_04[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    1,   0,     0,      0,  46 ),
};

const Sint32 pl13_special_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    1,   0,     0,      0,  46 ),
	CG_NEXT( pl13_special_04_loop ),
};

/* Char_Special 5 */
const Sint32 pl13_special_05[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    2,   0,     0,      0,  47 ),
};

const Sint32 pl13_special_05_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    2,   0,     0,      0,  47 ),
	CG_NEXT( pl13_special_05_loop ),
};

/* Char_Special 6: schwarz_start (Schwarzaile) */
const Sint32 pl13_special_06[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    3,   0,     0,      0,  48 ),
};

const Sint32 pl13_special_06_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    3,   0,     0,      0,  48 ),
	CG_NEXT( pl13_special_06_loop ),
};

/* Char_Special 7 */
const Sint32 pl13_special_07[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    4,   0,     0,      0,  49 ),
};

const Sint32 pl13_special_07_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    4,   0,     0,      0,  49 ),
	CG_NEXT( pl13_special_07_loop ),
};

/* Char_Special 8: schwarz_fly (Schwarzaile), schwarz_fall (Schwarzaile) */
const Sint32 pl13_special_08[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    89, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    89, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_special_08 ),
};

/* Char_Special 32: schwarz_fly (Schwarzaile) */
const Sint32 pl13_special_32[] = {
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl13_super_17_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   411, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_super_17_loop ),
};

/* Char_Super 0: flammeile_init (Flammeile (hyper)), flammeile_team_init (Flammeile as her part of a team hyper) */
const Sint32 pl13_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   403, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   403, 0x80,   0x00,  ATK,  0x00, 0x16,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,   3,           0,   403, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   404, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   405, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
};

const Sint32 pl13_super_00_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   405, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_super_00_loop ),
};

/* Char_Super 1: flammeile_next (Flammeile (hyper)), flammeile_auto (Flammeile (hyper)) */
const Sint32 pl13_super_01[] = {
	CG_FRAME(    0,    0,   3,           0,   407, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2,           0,   408, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   409, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl13_super_01 ),
};

/* Char_Super 2: flammeile_fly (Flammeile (hyper)) */
const Sint32 pl13_super_02[] = {
	CG_FRAME(    0,    0,   3,           0,   410, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   411, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   411, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl13_super_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    83, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_super_02_loop ),
};

/* Char_Super 3: flammeile_start (Flammeile (hyper)), flammeile_turn (Flammeile (hyper), 3 + routine_3), flammeile_team_start (Flammeile as her part of a team hyper) */
const Sint32 pl13_super_03[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00, 0x25,    0,   0,     0,      0,  50 ),
};

const Sint32 pl13_super_03_loop[] = {
	CG_FRAME(    0,    2,   1,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl13_super_03_loop ),
};

/* Char_Super 4 */
const Sint32 pl13_super_04[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00, 0x25,    1,   0,     0,      0,  51 ),
};

const Sint32 pl13_super_04_loop[] = {
	CG_FRAME(    0,    2,   1,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    1,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   6,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    1,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    1,   0,     0,      0,  51 ),
	CG_NEXT( pl13_super_04_loop ),
};

/* Char_Super 5 */
const Sint32 pl13_super_05[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00, 0x25,    2,   0,     0,      0,  52 ),
};

const Sint32 pl13_super_05_loop[] = {
	CG_FRAME(    0,    2,   1,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    2,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   6,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    2,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    2,   0,     0,      0,  52 ),
	CG_NEXT( pl13_super_05_loop ),
};

/* Char_Super 6 */
const Sint32 pl13_super_06[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00, 0x25,    3,   0,     0,      0,  53 ),
};

const Sint32 pl13_super_06_loop[] = {
	CG_FRAME(    0,    2,   1,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    3,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   6,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    3,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    3,   0,     0,      0,  53 ),
	CG_NEXT( pl13_super_06_loop ),
};

/* Char_Super 7 */
const Sint32 pl13_super_07[] = {
	CG_FRAME(    0,    1,   8,           0,   406, 0x80,   0x00,  ATK,  0x00, 0x25,    4,   0,     0,      0,  54 ),
};

const Sint32 pl13_super_07_loop[] = {
	CG_FRAME(    0,    2,   1,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    4,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6,           0,   406, 0x80,   0x00,  ATK,  0x00,    0,    4,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   406, 0x80,   0x00,  ATK,  0x00,    0,    4,   0,     0,      0,  54 ),
	CG_NEXT( pl13_super_07_loop ),
};

/* Char_Super 8: flammeile_fly (Flammeile (hyper)), flammeile_fall (Flammeile (hyper)), flammeile_team_fall (Flammeile as her part of a team hyper) */
const Sint32 pl13_super_08[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    89, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_super_08 ),
};

/* Char_Super 17: flammeile_fly (Flammeile (hyper)) */
const Sint32 pl13_super_17[] = {
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   411, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_super_17_loop ),
};

/* Char_Special 9: fantome_init (Fantome) */
const Sint32 pl13_special_09[] = {
	CG_FRAME(    0,    0,   3,           0,   398, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,  ATK,  0x00, 0x14,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   5,           0,   399, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   3,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   3,           0,   401, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,  12,           0,   402, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   1,           0,   181, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   3,           0,   181, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
};

const Sint32 pl13_special_09_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   101, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_special_09_loop ),
};

/* Char_Super 9: mille_init (Mille Fantomes (hyper)) */
const Sint32 pl13_super_09[] = {
	CG_FRAME(    0,    0,   2,           0,   398, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,  ATK,  0x00, 0x18,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,   1,           0,   398, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   8,           0,   398, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(   16,    0,   7,           0,   398, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,  ATK,  0x00, 0x1a,    0,   0,     0,      0,  13 ),
	CG_FRAME(   16,    0,   1,           0,   398, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   5,           0,   399, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    2,   3,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   401, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,  96,           0,   402, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,  80,           0,   402, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   8,           0,   181, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   7,           0,    99, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   6,           0,   100, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
};

const Sint32 pl13_super_09_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   101, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_super_09_loop ),
};

/* Char_Super 14: paretonnre_init (Pare-tonn're (hyper)) */
const Sint32 pl13_super_14[] = {
	CG_FRAME(    0,    0,   4,           0,   456, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    1,   1,           0,   456, 0x80,   0x00,  ATK,  0x00, 0x19,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   5,           0,   456, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    2,   1,           0,   456, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   457, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,   4,           0,   458, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   459, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   460, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,    94, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_super_14 ),
};

/* Char_Super 16: paretonnre_fall (Pare-tonn're (hyper)) */
const Sint32 pl13_super_16[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    89, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_super_16 ),
};

/* Char_Special 10: sublimation_init (Sublimation) */
const Sint32 pl13_special_10[] = {
	CG_FRAME(    0,    0,   3,           0,   388, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   389, 0x80,   0x00,  ATK,  0x00, 0x15,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   390, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   8,           0,   391, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   392, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,   3,           0,   393, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2,           0,   395, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  79 ),
};

const Sint32 pl13_special_10_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   394, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   395, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl13_special_10_loop ),
};

/* Char_Special 11: sublimation_wait (Sublimation) */
const Sint32 pl13_special_11[] = {
	CG_FRAME(    0,    0,   6,           0,   396, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   397, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_special_11 ),
};

/* Char_Special 12: rafale_throw_init (Rafale Cannon), throw_init (a throw that carries the opponent (assist)) */
const Sint32 pl13_special_12[] = {
	CG_FRAME(    0,    0,   4,           0,   369, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
};

const Sint32 pl13_special_12_loop[] = {
	CG_FRAME(    0,    1,   1,           0,   370, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   371, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   372, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   373, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   374, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   375, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   376, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   377, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_special_12_loop ),
};

/* Char_Special 13: throw_wait */
const Sint32 pl13_special_13[] = {
	CG_FRAME(    0,    0,   1,           0,   378, 0x80,   0x00,  ATK,  0x00, 0x23,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   378, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   379, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,   1,           0,   379, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   380, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,  12,           0,   381, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   8,           0,   382, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
};

const Sint32 pl13_special_13_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   175, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_special_13_loop ),
};

/* Char_Special 14: rafale_init (Rafale Cannon) */
const Sint32 pl13_special_14[] = {
	CG_FRAME(    0,    0,   8,           0,   311, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   312, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   313, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,  112,   1,           0,   313, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    2,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    3,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    4,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    5,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    6,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    7,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    8,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   2,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    9,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   2,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,   10,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,   11,   4, END_OF_DATA,   314, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl13_special_14 ),
};

/* Char_Special 15: rafale_shot (Rafale Cannon, 15 + sub_no), rafale_shot (Rafale Cannon) */
const Sint32 pl13_special_15[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,   10,   2,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 25 */
const Sint32 pl13_special_25[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    9,   2,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 24 */
const Sint32 pl13_special_24[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    8,   2,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 23 */
const Sint32 pl13_special_23[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    7,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 22 */
const Sint32 pl13_special_22[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    6,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 21 */
const Sint32 pl13_special_21[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    5,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 20 */
const Sint32 pl13_special_20[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    4,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 19 */
const Sint32 pl13_special_19[] = {
	CG_FRAME(    0,    3,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 18 */
const Sint32 pl13_special_18[] = {
	CG_FRAME(    0,    2,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 17 */
const Sint32 pl13_special_17[] = {
	CG_FRAME(    0,    1,   1,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
};

/* Char_Special 16 */
const Sint32 pl13_special_16[] = {
	CG_FRAME(    0,   12,   1, END_OF_DATA,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl13_special_15 ),
};

/* Char_Special 26: rafale_back (Rafale Cannon) */
const Sint32 pl13_special_26[] = {
	CG_FRAME(    0,    1,   6,           0,   316, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   6,           0,   317, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   6,           0,   318, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,  16,           0,   319, 0x80,   0x00,  ATE,  0x00, 0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   319, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_special_26 ),
};

/* Char_Special 27: rafale_catch (Rafale Cannon) */
const Sint32 pl13_special_27[] = {
	CG_FRAME(    0,    0,   6,           0,   319, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   319, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   14,   1,           0,   320, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   15,   1,           0,   321, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  15,           0,   321, 0x80,   0x00,  ATE,  0x00, 0x20,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   321, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_special_27 ),
};

/* Char_Special 28: rafale_hit (Rafale Cannon) */
const Sint32 pl13_special_28[] = {
	CG_FRAME(    0,    0,   6,           0,   322, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   6,           0,   323, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   6,           0,   324, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   6,           0,   325, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   326, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl13_special_28 ),
};

/* Char_Special 29: rafale_back (Rafale Cannon) */
const Sint32 pl13_special_29[] = {
	CG_FRAME(    0,    0,   2,           0,   327, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   328, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,  16,           0,   329, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   330, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_special_29 ),
};

/* Char_Special 30: power_init (a power-up for the partner (assist)) */
const Sint32 pl13_special_30[] = {
	CG_FRAME(    0,    0,   4,           0,   388, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   390, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,  24,           0,   391, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   392, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,   1,           0,   385, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   385, 0x80,   0x00,  ATE,  0x00, 0x0b,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   6,           0,   397, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   5,           0,    98, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_special_30 ),
};

/* Char_Super 10: magie_init (Tour De Magie (hyper)) */
const Sint32 pl13_super_10[] = {
	CG_FRAME(    0,    0,   4,           0,   388, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   388, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   388, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   389, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   433, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,  10,           0,   434, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   4,           0,   435, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,  10,           0,   437, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   439, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   175, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
};

const Sint32 pl13_super_10_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   101, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_super_10_loop ),
};

/* Char_Super 11: magie_catch (Tour De Magie (hyper)), magie_next (Tour De Magie (hyper)) */
const Sint32 pl13_super_11[] = {
	CG_FRAME(    0,    0,   3,           0,   427, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   427, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   428, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   429, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   430, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   431, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   432, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  24,           0,   427, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl13_super_11_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   427, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_super_11_loop ),
};

/* Char_Super 12: magie_next (Tour De Magie (hyper)) */
const Sint32 pl13_super_12[] = {
	CG_FRAME(    0,    0,   3,           0,   427, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   427, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   428, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   429, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   430, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   431, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   432, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl13_super_12_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   181, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_super_12_loop ),
};

/* Char_Super 13: magie_pick (Tour De Magie (hyper)), magie_result (Tour De Magie (hyper)), magie_miss (Tour De Magie (hyper)) */
const Sint32 pl13_super_13[] = {
	CG_FRAME(    0,    0,  20,           0,   100, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   123, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   123, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   125, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   126, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  20,           0,   127, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl13_super_13_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   127, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_super_13_loop ),
};

/* Char_Special 31: chouhatsu_init (the taunt) */
const Sint32 pl13_special_31[] = {
	CG_FRAME(    0,    0,   3,           0,   144, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  20,           0,   145, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   145, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl13_special_31_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_special_31_loop ),
};

/* Char_Extra 4
   Char_Change 4: act_change_assist_end */
const Sint32 pl13_extra_04[] = {
	CG_FRAME(    0,    0,   3,           0,   144, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  20,           0,   145, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   145, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl13_extra_04_loop[] = {
	CG_FRAME(    0,    0,  64, END_OF_DATA,   144, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_extra_04_loop ),
};

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   Char_Fly 0 */
const Sint32 pl13_stand_00[] = {
	CG_FRAME(    0,    0,   7,           0,    31, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    32, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    33, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    34, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    35, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    36, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    37, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    38, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    39, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    40, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,    41, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl13_stand_02[] = {
	CG_FRAME(    0,    0,   6,           0,    42, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    43, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    44, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    45, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    46, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    47, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    48, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    49, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    50, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    51, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl13_stand_03[] = {
	CG_FRAME(    0,    0,   6,           0,    52, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    53, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    54, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    55, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    56, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    57, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    58, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    59, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    60, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    61, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl13_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    31, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   9,           0,    70, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
};

const Sint32 pl13_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    77, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl13_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl13_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    31, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   9,           0,    82, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
};

const Sint32 pl13_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    77, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl13_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl13_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    31, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   9,           0,    80, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
};

const Sint32 pl13_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    77, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl13_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl13_jump_03[] = {
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_jump_03 ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl13_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    31, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl13_jump_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    83, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_jump_06_loop ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover
   Char_Super 15: paretonnre_wait (Pare-tonn're (hyper)) */
const Sint32 pl13_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl13_jump_09_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    83, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_jump_09_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl13_dash_00[] = {
	CG_FRAME(    0,    1,   2,           0,    62, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,    63, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl13_dash_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    62, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl13_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl13_dash_02[] = {
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    65, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl13_dash_01[] = {
	CG_FRAME(    0,    1,   2,           0,    66, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,    67, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl13_dash_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    66, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl13_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl13_dash_03[] = {
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl13_crouch_00[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl13_crouch_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    89, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl13_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl13_crouch_02[] = {
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl13_turn_00[] = {
	CG_FRAME(    0,    0,   2,           0,    96, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    97, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   101, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl13_turn_01[] = {
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   103, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   106, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl13_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl13_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   107, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl13_guard_01[] = {
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 2 */
const Sint32 pl13_guard_02[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   108, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_guard_01 ),
};

/* Char_Guard 4 */
const Sint32 pl13_guard_04[] = {
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl13_guard_05[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   110, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl13_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl13_guard_06[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   111, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl13_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl13_air_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl13_air_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   113, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_air_guard_01 ),
};

const Sint32 pl13_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3, END_OF_DATA,   113, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl13_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   170, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   3,           0,   172, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,   2,           0,   173, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   175, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl13_stand_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   176, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   2,           0,   178, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   6,           0,   179, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   180, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   175, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl13_stand_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   182, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   182, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   184, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   2,           0,   185, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   186, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,   3,           0,   187, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,  12,           0,   188, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   5,           0,   189, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl13_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   193, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl13_stand_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   196, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   8,           0,   197, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    1,   1,           0,   199, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME( 0x80,    0,   2,           0,   199, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0, 101 ),
	CG_FRAME( 0x80,    0,  10,           0,   200, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   8,           0,   194, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   193, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl13_stand_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,   202, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   202, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,   2,           0,   205, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME( 0x80,    0,   2,           0,   206, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,  10,           0,   207, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   8,           0,   208, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   170, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl13_crouch_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   210, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   211, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   3,           0,   212, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   211, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl13_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl13_crouch_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   214, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   215, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   3,           0,   216, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   8,           0,   217, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   5,           0,   218, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   219, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl13_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl13_crouch_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   1,           0,   222, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   3,           0,   224, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,  12,           0,   225, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   5,           0,   226, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   5,           0,   227, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   5,           0,   228, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    88, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl13_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl13_crouch_k_00[] = {
	CG_FRAME(    0,    0,   5,           0,   229, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   3,           0,   230, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   5,           0,   231, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   233, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    88, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl13_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl13_crouch_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   234, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   235, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   3,           0,   236, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   8,           0,   237, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    88, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl13_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl13_crouch_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,   240, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   242, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   243, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   2,           0,   244, 0x80,   0x00,  ATK,  0x00, 0x1f,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   7,           0,   245, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   247, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   5,           0,   249, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   5,           0,   250, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,  10,           0,   251, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   252, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl13_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl13_jump_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   253, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,   2,           0,   255, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   8,           0,   256, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   256, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl13_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl13_jump_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   257, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,   3,           0,   259, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,  10,           0,   260, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   261, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl13_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl13_jump_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   262, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0,   262, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   6,           0,   263, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   2,           0,   264, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   2,           0,   265, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,   3,           0,   266, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   3,           0,   267, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   267, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   267, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   267, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   267, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   267, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   267, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   261, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl13_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl13_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   269, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   2,           0,   270, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,  10,           0,   271, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
};

const Sint32 pl13_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   272, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl13_jump_k_00_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl13_jump_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,   273, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0,   274, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,   3,           0,   276, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  31 ),
};

const Sint32 pl13_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   277, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl13_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl13_jump_k_02[] = {
	CG_FRAME(    0,    0,   5,           0,   278, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   2,           0,   279, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   2,           0,   280, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,   3,           0,   281, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
};

const Sint32 pl13_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   282, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   283, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl13_jump_k_02_loop ),
};

/* Char_Extra 3: pl13_upper, fly_start5 */
const Sint32 pl13_extra_03[] = {
	CG_FRAME(    0,    0,   1,           0,   285, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   285, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   286, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   287, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   288, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,  10,           0,   289, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   5,           0,   290, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   5,           0,   291, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   3,           0,   292, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   293, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_extra_03 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl13_catch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,   8,           0,   295, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x00,  ATE,  0x00, 0x04,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,   2,           0,   298, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    1,   2,           0,   299, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   301, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   302, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   303, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   175, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_catch_00 ),
};

/* Char_Catch 1: jump_p_set */
const Sint32 pl13_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0008,   3 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0009,   3 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000a,   3 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000b,   3 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000c,   3 ),
	CG_FRAME(    0,    1,   4,           0,   309, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0, 0x000d,   3 ),
	CG_FRAME(    0,    0,   4,           0,   310, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   310, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_catch_01 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl13_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl13_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   116, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   121, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    90, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl13_get_up_02[] = {
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl13_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,   116, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_get_up_03 ),
};

/* Char_Damage 2 */
const Sint32 pl13_damage_02[] = {
	CG_FRAME(    0,    0,   6,           0,   485, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   484, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   483, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   482, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_damage_02 ),
};

/* Char_Damage 1 */
const Sint32 pl13_damage_01[] = {
	CG_FRAME(    0,    0,   7,           0,   484, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   483, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   482, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_damage_01 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl13_damage_00[] = {
	CG_FRAME(    0,    0,   5,           0,   483, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   482, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_damage_00 ),
};

/* Char_Damage 5 */
const Sint32 pl13_damage_05[] = {
	CG_FRAME(    0,    0,   6,           0,   489, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   488, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   486, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_damage_05 ),
};

/* Char_Damage 4 */
const Sint32 pl13_damage_04[] = {
	CG_FRAME(    0,    0,   7,           0,   488, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   486, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_damage_04 ),
};

/* Char_Damage 3 */
const Sint32 pl13_damage_03[] = {
	CG_FRAME(    0,    0,   5,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   486, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_damage_03 ),
};

/* Char_Damage 8 */
const Sint32 pl13_damage_08[] = {
	CG_FRAME(    0,    0,   6,           0,   493, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   7,           0,   492, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   491, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   490, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl13_damage_08 ),
};

/* Char_Damage 7 */
const Sint32 pl13_damage_07[] = {
	CG_FRAME(    0,    0,   7,           0,   492, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   491, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   490, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl13_damage_07 ),
};

/* Char_Damage 6 */
const Sint32 pl13_damage_06[] = {
	CG_FRAME(    0,    0,   5,           0,   491, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   490, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl13_damage_06 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl13_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   501, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   502, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   503, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   504, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   505, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   506, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   508, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   517, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   518, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   519, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   520, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl13_damage_09_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   521, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl13_damage_09_loop ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl13_damage_34[] = {
	CG_FRAME(    0,    0,   4,           0,   498, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,   499, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,   498, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,   499, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,   498, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,   499, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   519, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl13_damage_34 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl13_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   517, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2,           0,   518, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2,           0,   519, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
};

const Sint32 pl13_damage_13_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   520, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl13_damage_13_loop ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl13_damage_26[] = {
	CG_FRAME(    0,    0,   3,           0,   517, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0,   518, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0,   519, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   520, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl13_damage_26 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl13_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   521, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl13_damage_31 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl13_damage_10[] = {
	CG_FRAME(    0,    0,   6,           0,   495, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl13_damage_10_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   494, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_10_loop ),
};

/* Char_Damage 11 */
const Sint32 pl13_damage_11[] = {
	CG_FRAME(    0,    0,   4,           0,   496, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   495, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl13_damage_11_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   494, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_11_loop ),
};

/* Char_Damage 12 */
const Sint32 pl13_damage_12[] = {
	CG_FRAME(    0,    0,   4,           0,   497, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   496, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   495, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl13_damage_12_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   494, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_12_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl13_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   509, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   510, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl13_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   511, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl13_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   512, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl13_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   513, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl13_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   514, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl13_damage_21[] = {
	CG_FRAME(    0,    0,   4,           0,   509, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   510, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl13_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   511, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl13_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   512, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl13_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   513, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl13_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   514, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_25 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl13_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   511, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   513, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   522, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   523, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_damage_27 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl13_damage_33[] = {
	CG_FRAME(    0,    0,   3,           0,   511, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   513, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   522, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   523, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_damage_33 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl13_damage_28[] = {
	CG_FRAME(    0,    0,   3,           0,   524, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,   525, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,   526, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,   527, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,   528, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   529, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl13_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl13_damage_29[] = {
	CG_FRAME(    0,    0,   3,           0,   530, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,   531, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,   532, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,   533, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,   534, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   535, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl13_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl13_caught_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   483, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl13_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl13_caught_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   484, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl13_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl13_caught_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   485, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl13_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl13_caught_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl13_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl13_caught_04[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   488, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl13_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl13_caught_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   489, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl13_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl13_caught_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   491, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl13_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl13_caught_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   492, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl13_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl13_caught_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   493, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl13_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl13_caught_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl13_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl13_caught_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   496, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl13_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl13_caught_11[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   497, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl13_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl13_caught_12[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   498, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl13_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl13_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   510, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl13_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl13_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   511, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl13_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl13_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   512, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl13_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl13_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   513, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl13_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl13_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   514, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl13_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl13_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   524, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl13_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl13_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   530, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl13_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl13_caught_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   521, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl13_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl13_caught_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   521, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl13_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl13_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   486, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   482, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   486, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_dizzy_00 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl13_win_00[] = {
	CG_FRAME(    0,    0,   6,           0,   148, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   149, 0x80,   0x00,    0,  0x00, 0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   150, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   151, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   152, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   153, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   154, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   155, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   156, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   157, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   158, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  80,           0,   159, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl13_win_00_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   159, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_win_00_loop ),
};

/* Char_Win 1: win_scene_init */
const Sint32 pl13_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   398, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   401, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   402, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  48,           0,   101, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   164, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x00,    0,  0x00, 0x10,    0,   0,     0,      0,   1 ),
};

const Sint32 pl13_win_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   169, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_win_01_loop ),
};

const Sint32 pl13_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   160, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   164, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  80,           0,   168, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   169, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	0x000049b0,
};

/* Char_Win 2: lose_init */
const Sint32 pl13_win_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   482, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_win_02 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl13_change_00[] = {
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x00,  ATE,  0x00, 0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   123, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   125, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   126, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl13_change_00_loop[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   127, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl13_change_01[] = {
	CG_FRAME(    0,    0,   3,           0,   122, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   122, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl13_change_02[] = {
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl13_change_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    77, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl13_change_03[] = {
	CG_FRAME(    0,    0,   3,           0,   130, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x80,   0x00,    0,  0x00, 0x06,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl13_change_03_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   101, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_change_03_loop ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl13_appear_00[] = {
	CG_FRAME(    0,    0,   1,           0,   134, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x00,    0,  0x00, 0x0a,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   135, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   134, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   135, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   134, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   135, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   136, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   137, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   138, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   139, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   140, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   141, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   5,           0,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   141, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    96, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    97, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    98, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    99, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   100, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   101, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl13_appear_00 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl13_extra_00[] = {
	CG_FRAME(    0,    0,   3,           0,   269, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
};

const Sint32 pl13_extra_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   271, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl13_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl13_extra_01[] = {
	CG_FRAME(    0,    0,   8,           0,   101, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  24,           0,   129, 0x80,   0x00,  ATE,  0x00, 0x0c,    0,   0,     0,      0,   1 ),
};

const Sint32 pl13_extra_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   101, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl13_extra_01_loop ),
};

/* Char_Extra 2: snapback_init (the snapback), fly_start5 */
const Sint32 pl13_extra_02[] = {
	CG_FRAME(    0,    0,   3,           0,   182, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,   3,           0,   183, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    2,   3,           0,   184, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   3,           0,   185, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,   3,           0,   186, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME( 0x80,    0,   3,           0,   187, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,  12,           0,   188, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   5,           0,   189, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
};

const Sint32 pl13_extra_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl13_extra_02_loop ),
};

/* Char_Effect 0
   Char_Effect 28 */
const Sint32 pl13_effect_00[] = {
	CG_FRAME(    0,    0,  32,           0,   610, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   610, 0x80,   0x00,    0,  0x00, 0x21,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   610, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   610, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   610, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,    0,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,    0,  0x00,    0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,    0,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,    0,  0x00,    0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   612, 0x80,   0x00,    0,  0x00, 0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   613, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   614, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   616, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   617, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   613, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   614, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   616, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   617, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  32,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  32,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_00 ),
};

/* Char_Effect 56 */
const Sint32 pl13_effect_56[] = {
	CG_FRAME(    0,    0,  31,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   619, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   620, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   621, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   622, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   623, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   619, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   620, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   621, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   622, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   623, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   624, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  32,           0,   624, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  16, END_OF_DATA,   624, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_56 ),
};

/* Char_Effect 58 */
const Sint32 pl13_effect_58[] = {
	CG_FRAME(    0,    0,  16,           0,   610, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,    0,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,    0,  0x00,    0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,    0,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,    0,  0x00,    0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   612, 0x80,   0x00,    0,  0x00, 0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  10,           0,   613, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0,   614, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   616, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   617, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   613, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   614, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   616, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   617, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   6,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   6,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,  18,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  32,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_58 ),
};

/* Char_Effect 59 */
const Sint32 pl13_effect_59[] = {
	CG_FRAME(    0,    0,  15,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   619, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   620, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   621, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   622, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   623, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   619, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   620, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   621, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   622, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   623, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   624, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  32,           0,   624, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  16, END_OF_DATA,   624, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_59 ),
};

/* Char_Effect 1: ef_fantome_shot_fly, ef_mille_shot_fly */
const Sint32 pl13_effect_01[] = {
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,  ATK,  0x00, 0x1e,    0,   0,     0,      0,   0 ),
};

const Sint32 pl13_effect_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32797, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_NEXT( pl13_effect_01_loop ),
};

/* Char_Effect 57 */
const Sint32 pl13_effect_57[] = {
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,  ATK,  0x00, 0x1e,    0,   0,     0,      0,   0 ),
};

const Sint32 pl13_effect_57_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32797, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl13_effect_57_loop ),
};

/* Char_Effect 29: ef_fantome_shot_fly, ef_fantome_shot_hold, ef_mille_shot_fly */
const Sint32 pl13_effect_29[] = {
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32797, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_29 ),
};

/* Char_Effect 30: ef_fantome_shot_end, ef_mille_shot_end */
const Sint32 pl13_effect_30[] = {
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_30 ),
};

/* Char_Effect 2: ef_paretonnre_init */
const Sint32 pl13_effect_02[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32784, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_02 ),
};

/* Char_Effect 49: ef_paretonnre_atck_init */
const Sint32 pl13_effect_49[] = {
	CG_FRAME(    0,    1,  16,           0, 32792, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  36 ),
};

const Sint32 pl13_effect_49_loop[] = {
	CG_FRAME(    0,    2,   8, END_OF_DATA, 32792, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_NEXT( pl13_effect_49_loop ),
};

/* Char_Effect 3: ef_paretonnre_gun_init, ef_paretonnre_gun_shot */
const Sint32 pl13_effect_03[] = {
	CG_FRAME(    0,    0,   1,           0, 32785, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32786, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0, 32787, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32788, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32789, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0, 32789, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32790, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_03 ),
};

/* Char_Effect 60: ef_paretonnre_gun_init */
const Sint32 pl13_effect_60[] = {
	CG_FRAME(    0,    0,  10,           0, 32785, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32785, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32786, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0, 32787, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32788, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32789, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32790, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_60 ),
};

/* Char_Effect 4 */
const Sint32 pl13_effect_04[] = {
	CG_FRAME(    0,    1,   3,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    2,   3, END_OF_DATA, 32790, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl13_effect_04 ),
};

/* Char_Effect 55: ef_paretonnre_fire_init */
const Sint32 pl13_effect_55[] = {
	CG_FRAME(    0,    0,   1,           0,   671, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   672, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   673, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   674, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   675, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   676, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   677, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   678, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   679, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   680, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_55 ),
};

/* Char_Effect 5 */
const Sint32 pl13_effect_05[] = {
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   449, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   450, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   451, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   452, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   455, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl13_effect_06[] = {
	CG_FRAME(    0,    0,   3,           0,   452, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   451, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   450, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   449, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   448, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl13_effect_07[] = {
	CG_FRAME(    0,    0,  12,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl13_effect_07_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   454, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_07_loop ),
};

/* Char_Effect 8: ef_schwarz_front_init (8 + routine_3) */
const Sint32 pl13_effect_08[] = {
	CG_FRAME(    0,    0,   2,           0, 32771, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32776, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl13_effect_09[] = {
	CG_FRAME(    0,    0,   2,           0, 32771, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32776, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl13_effect_10[] = {
	CG_FRAME(    0,    0,   2,           0, 32771, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32776, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl13_effect_11[] = {
	CG_FRAME(    0,    0,   2,           0, 32771, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32776, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl13_effect_12[] = {
	CG_FRAME(    0,    0,   2,           0, 32771, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32776, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_12 ),
};

/* Char_Effect 13: ef_schwarz_mid_init (13 + routine_3) */
const Sint32 pl13_effect_13[] = {
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32770, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl13_effect_14[] = {
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32770, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl13_effect_15[] = {
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32770, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl13_effect_16[] = {
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32770, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl13_effect_17[] = {
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32770, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_17 ),
};

/* Char_Effect 18: ef_schwarz_back_init (18 + routine_3) */
const Sint32 pl13_effect_18[] = {
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32783, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl13_effect_19[] = {
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32783, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_19 ),
};

/* Char_Effect 20 */
const Sint32 pl13_effect_20[] = {
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32783, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_20 ),
};

/* Char_Effect 21 */
const Sint32 pl13_effect_21[] = {
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32783, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl13_effect_22[] = {
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32783, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_22 ),
};

/* Char_Effect 23: ef_magie_init */
const Sint32 pl13_effect_23[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   625, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl13_effect_23 ),
};

/* Char_Effect 47: ef_magie_fall */
const Sint32 pl13_effect_47[] = {
	CG_FRAME(    0,    0,   6,           0,   625, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   626, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   627, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   628, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_47 ),
};

/* Char_Effect 48: ef_magie_keep */
const Sint32 pl13_effect_48[] = {
	CG_FRAME(    0,    0,   8,           0,   625, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl13_effect_48_loop[] = {
	CG_FRAME(    0,    1,   2,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   625, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_48_loop ),
};

/* Char_Effect 44: ef_magie_lp_init, ef_magie_lk_init */
const Sint32 pl13_effect_44[] = {
	CG_FRAME(    0,    1,   7,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   1,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
};

const Sint32 pl13_effect_44_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   629, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_44_loop ),
};

/* Char_Effect 24: ef_magie_lp_fly */
const Sint32 pl13_effect_24[] = {
	CG_FRAME(    0,    0,   1,           0,   630, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   631, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   632, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   633, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_24 ),
};

/* Char_Effect 25: ef_magie_lk_fly */
const Sint32 pl13_effect_25[] = {
	CG_FRAME(    0,    0,   1,           0,   634, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   635, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   636, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   637, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_25 ),
};

/* Char_Effect 26: ef_magie_lp_arrive */
const Sint32 pl13_effect_26[] = {
	CG_FRAME(    0,    0,   6,           0,   633, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   647, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   648, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   649, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   638, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   639, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   640, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   641, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   646, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   642, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   643, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   644, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   645, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_26 ),
};

/* Char_Effect 27: ef_magie_lk_arrive */
const Sint32 pl13_effect_27[] = {
	CG_FRAME(    0,    0,   6,           0,   637, 0x80,   0x00,    0,  0x00, 0x26,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   659, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   660, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   661, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   650, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   651, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   652, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   653, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   658, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   654, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   655, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   656, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   657, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_27 ),
};

/* Char_Effect 45: ef_magie_obj_init */
const Sint32 pl13_effect_45[] = {
	CG_FRAME(    0,    0,   4,           0,   667, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   666, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   665, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   664, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   666, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   667, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_45 ),
};

/* Char_Effect 46: ef_magie_obj_keep */
const Sint32 pl13_effect_46[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   566, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_46 ),
};

/* Char_Effect 31: ef_sublimation_init */
const Sint32 pl13_effect_31[] = {
	CG_FRAME(    0,    1,  10,           0,    -1, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32793, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl13_effect_31_loop[] = {
	CG_FRAME(    0,    0,   3,           0, 32793, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32793, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl13_effect_31_loop ),
};

/* Char_Effect 32: ef_sublimation_part_init */
const Sint32 pl13_effect_32[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32794, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_32 ),
};

/* Char_Effect 33 */
const Sint32 pl13_effect_33[] = {
	CG_FRAME(    0,    0,   1,           0,   462, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   463, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   464, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   465, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   466, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   467, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   468, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   469, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   470, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   471, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   472, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   473, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   474, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   475, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   476, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   477, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   478, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   479, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   480, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   481, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_33 ),
};

/* Char_Effect 34: ef_throw_init */
const Sint32 pl13_effect_34[] = {
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x00,  ATK,  0x00,    0,    3,   0,     0,      0,   0 ),
};

const Sint32 pl13_effect_34_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x00,  ATK,  0x00,    0,    3,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   384, 0x80,   0x00,  ATK,  0x00,    0,    3,   0,     0,      0,  34 ),
	CG_NEXT( pl13_effect_34_loop ),
};

/* Char_Effect 35 */
const Sint32 pl13_effect_35[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   331, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_NEXT( pl13_effect_35 ),
};

/* Char_Effect 36 */
const Sint32 pl13_effect_36[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32768, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_36 ),
};

/* Char_Catch 2
   Char_Effect 37: ef_throw_fly (37 + no), ef_throw_fall (37 + no), ef_rafale_reach (37 + no) */
const Sint32 pl13_catch_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   333, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000e,   0 ),
	CG_NEXT( pl13_catch_02 ),
};

/* Char_Catch 3
   Char_Effect 38 */
const Sint32 pl13_catch_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   334, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000f,   0 ),
	CG_NEXT( pl13_catch_03 ),
};

/* Char_Catch 4
   Char_Effect 39 */
const Sint32 pl13_catch_04[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   335, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0010,   0 ),
	CG_NEXT( pl13_catch_04 ),
};

/* Char_Catch 5
   Char_Effect 40 */
const Sint32 pl13_catch_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   333, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0011,   0 ),
	CG_NEXT( pl13_catch_05 ),
};

/* Char_Catch 6
   Char_Effect 41 */
const Sint32 pl13_catch_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   334, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0012,   0 ),
	CG_NEXT( pl13_catch_06 ),
};

/* Char_Catch 7
   Char_Effect 42 */
const Sint32 pl13_catch_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   335, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0013,   0 ),
	CG_NEXT( pl13_catch_07 ),
};

/* Char_Effect 43: ef_power_init */
const Sint32 pl13_effect_43[] = {
	CG_FRAME(    0,    0,   1,           0,   542, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   543, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   544, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   545, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   546, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   547, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   548, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   549, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   550, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   551, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   552, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   553, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl13_effect_43 ),
};

/* Char_Effect 50: ef_flammeile_init (50 + routine_3) */
const Sint32 pl13_effect_50[] = {
	CG_FRAME(    0,    0,   9,           0,   670, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   670, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_50 ),
};

/* Char_Effect 51 */
const Sint32 pl13_effect_51[] = {
	CG_FRAME(    0,    0,   9,           0,   670, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   670, 0x80,   0x00,    0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_51 ),
};

/* Char_Effect 52 */
const Sint32 pl13_effect_52[] = {
	CG_FRAME(    0,    0,   9,           0,   670, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   670, 0x80,   0x00,    0,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_52 ),
};

/* Char_Effect 53 */
const Sint32 pl13_effect_53[] = {
	CG_FRAME(    0,    0,   9,           0,   670, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   670, 0x80,   0x00,    0,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_53 ),
};

/* Char_Effect 54 */
const Sint32 pl13_effect_54[] = {
	CG_FRAME(    0,    0,   9,           0,   670, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   670, 0x80,   0x00,    0,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_NEXT( pl13_effect_54 ),
};

/* group 27 0: pls13_00_turn_half_init
   group 28 0
   group 29 0
   group 30 0
   group 31 0
   group 32 0 */
const Sint32 pl13_grp27_00[] = {
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32780 ),
	CG_NEXT( pl13_grp27_00 ),
};

/* group 27 1: pls13_00_turn_init
   group 28 1
   group 29 1
   group 30 1
   group 31 1
   group 32 1 */
const Sint32 pl13_grp27_01[] = {
	EF_FRAME(    0,    0,   2,           0, 32777 ),
	EF_FRAME(    0,    0,   2,           0, 32778 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32779 ),
	CG_NEXT( pl13_grp27_01 ),
};

/* group 27 4: pls13_00_sublimation_init
   group 28 4
   group 29 4
   group 30 4
   group 31 4
   group 32 4 */
const Sint32 pl13_grp27_04[] = {
	EF_FRAME(    0,    0,   2,           0,   536 ),
	EF_FRAME(    0,    0,   2,           0,   537 ),
	EF_FRAME(    0,    0,   2,           0,   538 ),
	EF_FRAME(    0,    0,   2,           0,   539 ),
	EF_FRAME(    0,    0,   2,           0,   540 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   541 ),
	CG_NEXT( pl13_grp27_04 ),
};

/* group 27 7: pls13_00_t9_init
   group 28 7
   group 29 7
   group 30 7
   group 31 7
   group 32 7 */
const Sint32 pl13_grp27_07[] = {
	EF_FRAME(    0,    0,   1,           0,   561 ),
	EF_FRAME(    0,    0,   2,           0,   562 ),
	EF_FRAME(    0,    0,   2,           0,   563 ),
	EF_FRAME(    0,    0,   1,           0,   564 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   564 ),
	CG_NEXT( pl13_grp27_07 ),
};

/* group 27 8: pls13_00_t7_init
   group 28 8
   group 29 8
   group 30 8
   group 31 8
   group 32 8 */
const Sint32 pl13_grp27_08[] = {
	EF_FRAME(    0,    0,   2,           0,   566 ),
	EF_FRAME(    0,    0,   2,           0,   567 ),
	EF_FRAME(    0,    0,   1,           0,   568 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   568 ),
	CG_NEXT( pl13_grp27_08 ),
};

/* group 27 9: pls13_00_t11_init
   group 28 9
   group 29 9
   group 30 9
   group 31 9
   group 32 9 */
const Sint32 pl13_grp27_09[] = {
	EF_FRAME(    0,    0,   1,           0,   577 ),
	EF_FRAME(    0,    0,   2,           0,   578 ),
	EF_FRAME(    0,    0,   2,           0,   579 ),
	EF_FRAME(    0,    0,   2,           0,   580 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   581 ),
	CG_NEXT( pl13_grp27_09 ),
};

/* group 27 10: pls13_00_t10_init
   group 28 10
   group 29 10
   group 30 10
   group 31 10
   group 32 10 */
const Sint32 pl13_grp27_10[] = {
	EF_FRAME(    0,    0,   1,           0,   573 ),
	EF_FRAME(    0,    0,  24,           0,   574 ),
	EF_FRAME(    0,    1,  80,           0,   574 ),
	EF_FRAME(    0,    0,   4,           0,   575 ),
	EF_FRAME(    0,    0,   4,           0,   576 ),
	EF_FRAME(    0,    2,   4,           0,   576 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   576 ),
	CG_NEXT( pl13_grp27_10 ),
};

/* group 27 11: pls13_00_t8_init
   group 28 11
   group 29 11
   group 30 11
   group 31 11
   group 32 11 */
const Sint32 pl13_grp27_11[] = {
	EF_FRAME(    0,    0,   1,           0,   569 ),
	EF_FRAME(    0,    0,   2,           0,   570 ),
	EF_FRAME(    0,    0,   2,           0,   571 ),
	EF_FRAME(    0,    0,   6,           0,   572 ),
	EF_FRAME(    0,    1,   1,           0,   572 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   572 ),
	CG_NEXT( pl13_grp27_11 ),
};

/* group 27 12: pls13_00_t12_init
   group 28 12
   group 29 12
   group 30 12
   group 31 12
   group 32 12 */
const Sint32 pl13_grp27_12[] = {
	EF_FRAME(    0,    0,   1,           0,   565 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   565 ),
	CG_NEXT( pl13_grp27_12 ),
};

/* group 27 2: pls13_00_magie_upper_init
   group 28 2
   group 29 2
   group 30 2
   group 31 2
   group 32 2 */
const Sint32 pl13_grp27_02[] = {
	EF_FRAME(    0,    0,   8,           0,   662 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,    -1 ),
	CG_NEXT( pl13_grp27_02 ),
};

/* group 27 5: pls13_00_magie_lower_init
   group 28 5
   group 29 5
   group 30 5
   group 31 5
   group 32 5 */
const Sint32 pl13_grp27_05[] = {
	EF_FRAME(    0,    0,   8,           0,   663 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,    -1 ),
	CG_NEXT( pl13_grp27_05 ),
};

/* group 27 3: pls13_00_magie_upper_init
   group 28 3
   group 29 3
   group 30 3
   group 31 3
   group 32 3 */
const Sint32 pl13_grp27_03[] = {
	EF_FRAME(    0,    0,   8,           0,   668 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,    -1 ),
	CG_NEXT( pl13_grp27_03 ),
};

/* group 27 6: pls13_00_magie_lower_init
   group 28 6
   group 29 6
   group 30 6
   group 31 6
   group 32 6 */
const Sint32 pl13_grp27_06[] = {
	EF_FRAME(    0,    0,   8,           0,   669 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,    -1 ),
	CG_NEXT( pl13_grp27_06 ),
};

/* group 27 13: pls13_00_t15_init
   group 28 13
   group 29 13
   group 30 13
   group 31 13
   group 32 13 */
const Sint32 pl13_grp27_13[] = {
	EF_FRAME(    0,    0,  12,           0,   582 ),
	EF_FRAME(    0,    0,   3,           0,   602 ),
	EF_FRAME(    0,    0,   3,           0,   601 ),
	EF_FRAME(    0,    0,   3,           0,   600 ),
	EF_FRAME(    0,    1,   3,           0,   600 ),
};

const Sint32 pl13_grp27_13_loop[] = {
	EF_FRAME(    0,    0,   2,           0,   583 ),
	EF_FRAME(    0,    0,   2,           0,   584 ),
	EF_FRAME(    0,    0,   2,           0,   585 ),
	EF_FRAME(    0,    0,   2,           0,   586 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   587 ),
	CG_NEXT( pl13_grp27_13_loop ),
};

/* group 27 14: pls13_00_appear_init, pls13_00_t14_init
   group 28 14
   group 29 14
   group 30 14
   group 31 14
   group 32 14 */
const Sint32 pl13_grp27_14[] = {
	EF_FRAME(    0,    0,  64, END_OF_DATA,   610 ),
	CG_NEXT( pl13_grp27_14 ),
};
