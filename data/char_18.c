/* Abyss (first form) (character 18): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_18      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_18     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl18.c) says which group
                   and number each move uses.
   pl18_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl18_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl18_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl18_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_18
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_18, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_18 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl18_stand_00[], pl18_stand_02[], pl18_stand_03[], pl18_dash_00[],
	pl18_turn_00[], pl18_guard_00[], pl18_stand_p_00[], pl18_damage_01[], pl18_damage_09[],
	pl18_damage_28[], pl18_appear_00[], pl18_win_00[], pl18_win_00_loop[], pl18_win_01[],
	pl18_special_00[], pl18_special_01[], pl18_special_02[], pl18_special_03[],
	pl18_special_03_loop[], pl18_special_04[], pl18_special_05[], pl18_special_05_loop[],
	pl18_special_06[], pl18_special_07[], pl18_special_07_loop[], pl18_special_08[],
	pl18_effect_00[], pl18_effect_01[], pl18_effect_02[], pl18_effect_03[], pl18_effect_04[],
	pl18_effect_05[], pl18_effect_06[], pl18_effect_07[], pl18_change_00[], pl18_grp25_00[],
	pl18_grp25_01[], pl18_grp25_02[], pl18_grp25_03[], pl18_grp25_04[], pl18_grp25_05[],
	pl18_grp25_06[], pl18_grp25_07[], pl18_grp25_08[], pl18_grp25_09[], pl18_grp25_10[],
	pl18_grp25_11[], pl18_grp25_12[], pl18_grp25_13[], pl18_grp25_14[], pl18_grp25_15[],
	pl18_grp25_16[], pl18_grp25_17[], pl18_grp25_18[], pl18_grp25_19[], pl18_grp25_20[],
	pl18_grp25_21[], pl18_grp25_22[], pl18_grp25_23[], pl18_grp27_00[], pl18_grp27_01[],
	pl18_grp27_02[], pl18_grp27_03[], pl18_grp27_04[], pl18_grp27_05[], pl18_grp27_05_loop[],
	pl18_grp27_06[], pl18_grp27_07[], pl18_grp27_09[], pl18_grp27_10[], pl18_grp27_11[],
	pl18_grp27_12[], pl18_grp27_13[], pl18_grp27_14[], pl18_grp27_16[], pl18_grp27_17[],
	pl18_grp27_18[], pl18_grp27_19[], pl18_grp27_20[], pl18_grp27_21[], pl18_grp27_22[],
	pl18_grp27_23[], pl18_grp27_24[], pl18_grp27_25[], pl18_grp27_26[], pl18_grp27_27[],
	pl18_grp27_28[], pl18_grp27_29[], pl18_grp27_30[], pl18_grp27_31[], pl18_grp27_32[],
	pl18_grp27_33[], pl18_grp27_34[], pl18_grp27_35[], pl18_grp27_36[], pl18_grp27_37[],
	pl18_grp27_38[], pl18_grp27_39[], pl18_grp27_40[], pl18_grp27_41[], pl18_grp27_42[],
	pl18_grp27_47[], pl18_grp27_43[], pl18_grp27_44[], pl18_grp27_45[], pl18_grp27_48[];
extern const Sint32 *const pl18_stand_tbl[], *const pl18_jump_tbl[], *const pl18_dash_tbl[],
	*const pl18_crouch_tbl[], *const pl18_turn_tbl[], *const pl18_guard_tbl[],
	*const pl18_air_guard_tbl[], *const pl18_stand_p_tbl[], *const pl18_stand_k_tbl[],
	*const pl18_crouch_p_tbl[], *const pl18_crouch_k_tbl[], *const pl18_jump_p_tbl[],
	*const pl18_jump_k_tbl[], *const pl18_damage_tbl[], *const pl18_dizzy_tbl[],
	*const pl18_catch_tbl[], *const pl18_caught_tbl[], *const pl18_get_up_tbl[],
	*const pl18_appear_tbl[], *const pl18_win_tbl[], *const pl18_extra_tbl[],
	*const pl18_special_tbl[], *const pl18_super_tbl[], *const pl18_effect_tbl[],
	*const pl18_change_tbl[], *const pl18_grp25_tbl[], *const pl18_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_18[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 900, 900, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -4096, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_18[] = {
	pl18_stand_tbl, pl18_jump_tbl, pl18_dash_tbl, pl18_crouch_tbl, pl18_turn_tbl, pl18_guard_tbl,
	pl18_air_guard_tbl, pl18_stand_p_tbl, pl18_stand_k_tbl, pl18_crouch_p_tbl, pl18_crouch_k_tbl, pl18_jump_p_tbl,
	pl18_jump_k_tbl, pl18_damage_tbl, pl18_dizzy_tbl, pl18_catch_tbl, pl18_caught_tbl, pl18_get_up_tbl,
	pl18_appear_tbl, pl18_win_tbl, pl18_extra_tbl, pl18_special_tbl, pl18_super_tbl, pl18_effect_tbl,
	pl18_change_tbl, pl18_grp25_tbl, pl18_grp25_tbl, pl18_grp27_tbl, 0,
};

const Sint32 *const pl18_stand_tbl[] = {
	pl18_stand_00, pl18_stand_00, pl18_stand_02, pl18_stand_03, pl18_stand_00, pl18_stand_00,
	pl18_stand_03, pl18_stand_03, 0,
};

const Sint32 *const pl18_jump_tbl[] = {
	pl18_stand_03, pl18_stand_03, pl18_stand_03, pl18_stand_03, pl18_stand_03, pl18_stand_03,
	pl18_stand_03, pl18_stand_03, pl18_stand_03, pl18_stand_03, pl18_stand_03, pl18_stand_03,
	0,
};

const Sint32 *const pl18_dash_tbl[] = {
	pl18_dash_00, pl18_dash_00, pl18_dash_00, pl18_dash_00, 0,
};

const Sint32 *const pl18_crouch_tbl[] = {
	pl18_dash_00, pl18_dash_00, pl18_dash_00, 0,
};

const Sint32 *const pl18_turn_tbl[] = {
	pl18_turn_00, pl18_turn_00, 0,
};

const Sint32 *const pl18_guard_tbl[] = {
	pl18_guard_00, pl18_guard_00, pl18_guard_00, pl18_guard_00, pl18_guard_00, pl18_guard_00,
	pl18_guard_00, pl18_guard_00, 0,
};

const Sint32 *const pl18_air_guard_tbl[] = {
	pl18_guard_00, pl18_guard_00, pl18_guard_00, 0,
};

const Sint32 *const pl18_stand_p_tbl[] = {
	pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, 0,
};

const Sint32 *const pl18_stand_k_tbl[] = {
	pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, 0,
};

const Sint32 *const pl18_crouch_p_tbl[] = {
	pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, 0,
};

const Sint32 *const pl18_crouch_k_tbl[] = {
	pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, 0,
};

const Sint32 *const pl18_jump_p_tbl[] = {
	pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00,
	0,
};

const Sint32 *const pl18_jump_k_tbl[] = {
	pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00,
	0,
};

const Sint32 *const pl18_damage_tbl[] = {
	pl18_stand_p_00, pl18_damage_01, pl18_damage_01, pl18_damage_01, pl18_damage_01, pl18_damage_01,
	pl18_damage_01, pl18_damage_01, pl18_damage_01, pl18_damage_09, pl18_damage_01, pl18_damage_01,
	pl18_damage_01, pl18_damage_09, pl18_damage_09, pl18_damage_09, pl18_damage_09, pl18_damage_09,
	pl18_damage_09, pl18_damage_09, pl18_damage_09, pl18_damage_09, pl18_damage_09, pl18_damage_09,
	pl18_damage_09, pl18_damage_09, pl18_damage_09, pl18_damage_09, pl18_damage_28, pl18_damage_28,
	pl18_damage_09, pl18_damage_09, pl18_damage_28, pl18_damage_28, pl18_damage_09, 0,
};

const Sint32 *const pl18_dizzy_tbl[] = {
	pl18_damage_28, 0,
};

const Sint32 *const pl18_catch_tbl[] = {
	pl18_stand_00, 0,
};

const Sint32 *const pl18_caught_tbl[] = {
	pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28,
	pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28,
	pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28,
	pl18_damage_28, pl18_damage_28, pl18_damage_28, pl18_damage_28, 0,
};

const Sint32 *const pl18_get_up_tbl[] = {
	pl18_stand_p_00, pl18_stand_p_00, pl18_stand_p_00, 0,
};

const Sint32 *const pl18_appear_tbl[] = {
	pl18_appear_00, 0,
};

const Sint32 *const pl18_win_tbl[] = {
	pl18_win_00, pl18_win_01, pl18_win_01, 0,
};

const Sint32 *const pl18_extra_tbl[] = {
	pl18_stand_00, 0,
};

const Sint32 *const pl18_special_tbl[] = {
	pl18_special_00, pl18_special_01, pl18_special_02, pl18_special_03, pl18_special_04, pl18_special_05,
	pl18_special_06, pl18_special_07, pl18_special_08, 0,
};

const Sint32 *const pl18_super_tbl[] = {
	pl18_stand_00, 0,
};

const Sint32 *const pl18_effect_tbl[] = {
	pl18_effect_00, pl18_effect_01, pl18_effect_02, pl18_effect_03, pl18_effect_04, pl18_effect_05,
	pl18_effect_06, pl18_effect_07, 0,
};

const Sint32 *const pl18_change_tbl[] = {
	pl18_change_00, pl18_change_00, pl18_change_00, pl18_change_00, pl18_change_00, pl18_change_00,
	0,
};

const Sint32 *const pl18_grp25_tbl[] = {
	pl18_grp25_00, pl18_grp25_01, pl18_grp25_02, pl18_grp25_03, pl18_grp25_04, pl18_grp25_05,
	pl18_grp25_06, pl18_grp25_07, pl18_grp25_08, pl18_grp25_09, pl18_grp25_10, pl18_grp25_11,
	pl18_grp25_12, pl18_grp25_13, pl18_grp25_14, pl18_grp25_15, pl18_grp25_16, pl18_grp25_17,
	pl18_grp25_18, pl18_grp25_19, pl18_grp25_20, pl18_grp25_21, pl18_grp25_22, pl18_grp25_23,
	0,
};

const Sint32 *const pl18_grp27_tbl[] = {
	pl18_grp27_00, pl18_grp27_01, pl18_grp27_02, pl18_grp27_03, pl18_grp27_04, pl18_grp27_05,
	pl18_grp27_06, pl18_grp27_07, pl18_grp27_06, pl18_grp27_09, pl18_grp27_10, pl18_grp27_11,
	pl18_grp27_12, pl18_grp27_13, pl18_grp27_14, pl18_grp27_00, pl18_grp27_16, pl18_grp27_17,
	pl18_grp27_18, pl18_grp27_19, pl18_grp27_20, pl18_grp27_21, pl18_grp27_22, pl18_grp27_23,
	pl18_grp27_24, pl18_grp27_25, pl18_grp27_26, pl18_grp27_27, pl18_grp27_28, pl18_grp27_29,
	pl18_grp27_30, pl18_grp27_31, pl18_grp27_32, pl18_grp27_33, pl18_grp27_34, pl18_grp27_35,
	pl18_grp27_36, pl18_grp27_37, pl18_grp27_38, pl18_grp27_39, pl18_grp27_40, pl18_grp27_41,
	pl18_grp27_42, pl18_grp27_43, pl18_grp27_44, pl18_grp27_45, pl18_grp27_45, pl18_grp27_47,
	pl18_grp27_48, 0,
};

	/*        prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Catch 0
   Char_Extra 0: tk_init */
const Sint32 pl18_stand_00[] = {
	CG_FRAME(   0,    0,  13,           0,    38, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    39, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    40, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    41, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    42, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    43, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    44, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    45, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   7,           0,    46, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    47, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    48, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    47, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    46, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   7,           0,    45, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    44, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    43, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    42, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   7,           0,    41, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    40, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    39, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  13,           0,    49, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    50, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    51, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    52, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    53, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    54, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    55, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    56, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   7,           0,    57, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    58, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    59, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    58, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    57, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   7,           0,    56, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    55, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    54, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    53, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   7,           0,    52, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    51, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    50, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  13,           0,    49, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    50, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    51, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    52, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    53, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    54, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    55, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    56, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   7,           0,    57, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    58, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    59, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    58, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    57, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   7,           0,    56, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    55, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    54, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    53, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   7,           0,    52, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    51, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    50, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  13, END_OF_DATA,    49, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl18_stand_02[] = {
	CG_FRAME(   0,    0,   5,           0,    99, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,   100, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,   101, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    82, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    83, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    84, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    85, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   5,           0,    86, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,  10,           0,    87, 0x40,   0x00,    0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    88, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    89, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    90, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    91, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    92, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    93, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    94, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,    95, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   5,           0,    96, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,  10,           0,    97, 0x40,   0x00,    0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5, END_OF_DATA,    98, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 6
   Char_Stand 7
   Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2 */
const Sint32 pl18_stand_03[] = {
	CG_FRAME(   0,    0,   3, END_OF_DATA,    38, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_stand_03 ),
};

/* Char_Dash 0: act_dash
   Char_Dash 1: act_back_dash
   Char_Dash 2
   Char_Dash 3
   Char_Crouch 0: act_crouch, act_stand_up
   Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl18_dash_00[] = {
	CG_FRAME(   0,    0,   3, END_OF_DATA,    38, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_dash_00 ),
};

/* Char_Turn 0: act_turn
   Char_Turn 1: act_crouch_turn */
const Sint32 pl18_turn_00[] = {
	CG_FRAME(   0,    0,   3,           0,    60, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    61, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    62, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    63, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,    64, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3, END_OF_DATA,    65, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_turn_00 ),
};

/* Char_Guard 0
   Char_Guard 1: act_guard_hold
   Char_Guard 2
   Char_Guard 3
   Char_Guard 4
   Char_Guard 5: act_guard_hold */
const Sint32 pl18_guard_00[] = {
	CG_FRAME(   0,    0,   3, END_OF_DATA,    38, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_guard_00 ),
};

/* Char_Stand_P 0 (light)
   Char_Stand_P 1 (medium)
   Char_Stand_P 2 (heavy)
   Char_Stand_K 0 (light)
   Char_Stand_K 1 (medium)
   Char_Stand_K 2 (heavy) */
const Sint32 pl18_stand_p_00[] = {
	CG_FRAME(   0,    0,   3, END_OF_DATA,    38, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_stand_p_00 ),
};

/* Char_Damage 1
   Char_Damage 2
   Char_Damage 3
   Char_Damage 4
   Char_Damage 5
   Char_Damage 6 */
const Sint32 pl18_damage_01[] = {
	CG_FRAME(   0,    0,   4,           0,   153, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   154, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   153, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   154, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   155, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   156, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   157, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   158, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   159, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   160, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   161, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   162, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,    75, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,    65, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_stand_p_00 ),
};

/* Char_Damage 9: dm_sweep
   Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init
   Char_Damage 17: dm_07_1 */
const Sint32 pl18_damage_09[] = {
	CG_FRAME(   0,    0,   4, END_OF_DATA,   185, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl18_damage_09 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away
   Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ...
   Char_Damage 32: act_escape
   Char_Damage 33: act_escape
   Char_Dizzy 0: act_dizzy
   Char_Caught 0 */
const Sint32 pl18_damage_28[] = {
	CG_FRAME(   0,    0,   3, END_OF_DATA,    38, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_damage_28 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl18_appear_00[] = {
	CG_FRAME(   0,    0,   2,           0,   272, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   273, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   274, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   275, 0x40,   0x00,    0,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   276, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   277, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   278, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,   279, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   5,           0,   280, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   281, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   282, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   7,           0,   283, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   1,    1,   7,           0,   284, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   2,    1,   8,           0,   285, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   286, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,    55, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,    54, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,    53, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,    52, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   9,           0,    51, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  11,           0,    50, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  13, END_OF_DATA,    49, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_stand_00 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl18_win_00[] = {
	CG_FRAME(   0,    0,   4,           0,    76, 0x40,   0x47,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   211, 0x40,   0x47,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   212, 0x40,   0x47,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   5,           0,   213, 0x40,   0x47,    0,  0x00, 0x16 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   5,           0,   213, 0x40,   0x47,    0,  0x00, 0x16 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   5,           0,   213, 0x40,   0x47,    0,  0x00, 0x16 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   5,           0,   213, 0x40,   0x47,    0,  0x00, 0x16 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(   1,    1,   4,           0,   214, 0x40,   0x47,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   2,    1,   4,           0,   215, 0x40,   0x47,    0,  0x00, 0x31 | CSE,    0,   0,     0,      0,   1 ),
};

const Sint32 pl18_win_00_loop[] = {
	CG_FRAME(   3,    1,   4, END_OF_DATA,   216, 0x40,   0x47,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_win_00_loop ),
};

/* Char_Win 1
   Char_Win 2 */
const Sint32 pl18_win_01[] = {
	CG_FRAME(   0,    0,   8,           0,   163, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   164, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   165, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   166, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   167, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   168, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   169, 0x40,   0x4d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   170, 0x40,   0x00,    0,  0x00, 0x30 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   171, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   172, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   173, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   174, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   175, 0x40,   0x00,    0,  0x00, 0x31 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   176, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   177, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   178, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   179, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   180, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   181, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   182, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   183, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   184, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   185, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   186, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   187, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   188, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   189, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   190, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   191, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   192, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   193, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   194, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   195, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   196, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   197, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   198, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   199, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   200, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   201, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   202, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   203, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   204, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   205, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   206, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   207, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   208, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   209, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,   210, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_win_01 ),
};

/* Char_Special 0: punch_init (a dark fist thrown out) */
const Sint32 pl18_special_00[] = {
	CG_FRAME(   0,    0,   8,           0,   102, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   103, 0x40,   0x00,  ATK,  0x00, 0x23 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   104, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   105, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   106, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   107, 0x40,   0x00,  ATK,  0x00,       0x17,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   107, 0x40,   0x00,  ATK,  0x00,       0x01,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   108, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   109, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   110, 0x40,   0x4d,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(   0,    0,   7,           0,   111, 0x40,   0x4d,  ATK,  0x00,       0x04,    0,   0,     0,      0,  20 ),
	CG_FRAME(   0,    1,   6,           0,   112, 0x40,   0x4d,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(   1,    1,   5,           0,   113, 0x40,   0x4d,  ATK,  0x00,          0,    0,   0,     0, 0xfff8,  20 ),
	CG_FRAME(   2,    1,   4,           0,   114, 0x40,   0x55,  ATK,  0x00,          0,    0,   0,     0, 0xffe0,  21 ),
	CG_FRAME(   3,    2,   4,           0,   115, 0x40,   0x55,  ATK,  0x00,       0x18,    0,   0,     0, 0xfff8,  19 ),
	CG_FRAME(   0,    0,   6,           0,   116, 0x40,   0x55,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(   0,    0,   7,           0,   117, 0x40,   0x55,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(   0,    0,   8,           0,   118, 0x40,   0x55,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(   0,    0,  10,           0,   119, 0x40,   0x13,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(   0,    0,   8,           0,    75, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8, END_OF_DATA,    65, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_special_00 ),
};

/* Char_Special 1: thunder_init (dark bolts dropped on the opponent (hyper)) */
const Sint32 pl18_special_01[] = {
	CG_FRAME(   0,    0,   1,           0,    76, 0x40,   0x47,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  13,           0,    76, 0x40,   0x47,  ATK,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    77, 0x40,   0x47,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    78, 0x40,   0x47,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  48,           0,    79, 0x40,   0x47,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   6,           0,    79, 0x40,   0x47,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,  32,           0,    79, 0x40,   0x47,  ATK,  0x00,       0x19,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  32,           0,    79, 0x40,   0x47,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,    78, 0x40,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,    80, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,    81, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   1, END_OF_DATA,    65, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_special_01 ),
};

/* Char_Special 2: ball_init (three homing dark balls) */
const Sint32 pl18_special_02[] = {
	CG_FRAME(  16,    0,   6,           0,   129, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0xfff0,   1 ),
	CG_FRAME(  17,    0,   6,           0,   130, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  18,    0,   6,           0,   131, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  19,    0,   6,           0,   132, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  21,    0,   6,           0,   133, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  20,    1,   6,           0,   134, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  21,    0,   6,           0,   133, 0x40,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(  20,    1,   6,           0,   134, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  21,    0,   6,           0,   133, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  20,    1,   6,           0,   134, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  21,    0,   6,           0,   133, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  20,    0,   6,           0,   134, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   133, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   134, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   133, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   134, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   133, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   134, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   133, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   134, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   133, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   134, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   133, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   134, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   135, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   136, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6, END_OF_DATA,    65, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0010,   1 ),
	CG_NEXT( pl18_special_02 ),
};

/* Char_Special 3: pillar_init (a row of dark pillars (hyper)) */
const Sint32 pl18_special_03[] = {
	CG_FRAME(   0,    0,   6,           0,   142, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   143, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  32,           0,   144, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   1,    0,  64,           0,   145, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   6,           0,   146, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   2,           0,   147, 0x40,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   2,           0,   147, 0x40,   0x00,  ATK,  0x00, 0x48 | CSE,    0,   0,     0,      0,   1 ),
};

const Sint32 pl18_special_03_loop[] = {
	CG_FRAME(  17,    0,   4,           0,   148, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  18,    0,   4,           0,   149, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  16,    0,   4,           0,   150, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(  18,    0,   4, END_OF_DATA,   151, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_special_03_loop ),
};

/* Char_Special 4: pillar_hold (a row of dark pillars (hyper)) */
const Sint32 pl18_special_04[] = {
	CG_FRAME(   0,    0,   4,           0,   148, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   149, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   150, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   151, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   148, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   149, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   150, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   151, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   152, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,    75, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8, END_OF_DATA,    65, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_special_04 ),
};

/* Char_Special 5: change_init (his fall, to change into the second form) */
const Sint32 pl18_special_05[] = {
	CG_FRAME(   0,    0,   8,           0,   163, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   8,           0,   164, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   8,           0,   165, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   8,           0,   166, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   8,           0,   167, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   8,           0,   168, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   8,           0,   169, 0x40,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    1,   8,           0,   170, 0x40,   0x0b,    0,  0x00, 0x30 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   171, 0x40,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   172, 0x40,   0x0a,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   173, 0x40,   0x47,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   174, 0x40,   0xa2,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   1,    1,   4,           0,   175, 0x40,   0xe1,    0,  0x00, 0x31 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   176, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   177, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   178, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   179, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   180, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   181, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   182, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   183, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   184, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl18_special_05_loop[] = {
	CG_FRAME(   0,    0,   4, END_OF_DATA,   185, 0x40,   0xe1,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl18_special_05_loop ),
};

/* Char_Special 6: sweep_init (a long sweeping blow) */
const Sint32 pl18_special_06[] = {
	CG_FRAME(   0,    0,   5,           0,    66, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(   0,    1,   8,           0,    67, 0x40,   0x0f,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(   1,    1,   6,           0,    68, 0x40,   0x57,  ATK,  0x00,          0,    0,   0,     0, 0xfff8,  17 ),
	CG_FRAME(   2,    1,   6,           0,    69, 0x40,   0x57,  ATK,  0x00,       0x03,    0,   0,     0, 0xffe0,  18 ),
	CG_FRAME(   3,    2,   5,           0,    70, 0x40,   0x57,  ATK,  0x00, 0x4b | CSE,    0,   0,     0, 0xfffc,  16 ),
	CG_FRAME(   4,    1,   4,           0,    71, 0x40,   0x57,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   5,    1,   4,           0,    72, 0x40,   0x57,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   6,    1,   4,           0,    73, 0x40,   0x57,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(   7,    1,   4,           0,    74, 0x40,   0x54,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(   8,    1,   4,           0,    75, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  16,           0,    75, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6, END_OF_DATA,    65, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_special_06 ),
};

/* Char_Special 7: rush_init (a charging rush) */
const Sint32 pl18_special_07[] = {
	CG_FRAME(   0,    1,  32,           0,   120, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   8,           0,   121, 0x40,   0x12,  ATK,  0x00, 0x2f | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   122, 0x40,   0x16,  ATK,  0x00, 0x4c | CSE,    0,   0,     0,      0,  24 ),
};

const Sint32 pl18_special_07_loop[] = {
	CG_FRAME(  16,    0,   2,           0,   123, 0x40,   0x7d,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(  17,    1,   2, END_OF_DATA,   124, 0x40,   0x7d,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl18_special_07_loop ),
};

/* Char_Special 8: rush_keep (a charging rush) */
const Sint32 pl18_special_08[] = {
	CG_FRAME(   0,    0,   8,           0,   123, 0x40,   0x7d,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  32,           0,   124, 0x40,   0x7d,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  16,           0,   125, 0x40,   0x5b,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  10,           0,   126, 0x40,   0x18,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   8,           0,   127, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   8, END_OF_DATA,    65, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_special_08 ),
};

/* Char_Effect 0 */
const Sint32 pl18_effect_00[] = {
	CG_FRAME(   0,    0,   1,           0,   355, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(   0,    0,   2,           0,   344, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(   0,    0,   2,           0,   345, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(   0,    0,   2,           0,   346, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(   0,    1,   4,           0,   347, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   348, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   349, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   350, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   351, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   352, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   353, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,   354, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl18_effect_00 ),
};

/* Char_Effect 1: shl18_00_init */
const Sint32 pl18_effect_01[] = {
	CG_FRAME(   0,    0,   2,           0, 32768, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(   0,    0,   2,           0, 32769, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(   0,    0,   2,           0, 32770, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(   0,    0,   2,           0, 32771, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(   0,    0,   2,           0, 32772, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(   0,    0,   2,           0, 32773, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(   0,    0,   2,           0, 32774, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(   0,    0,   2,           0, 32775, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(   0,    0,   2,           0, 32776, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32777, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl18_effect_01 ),
};

/* Char_Effect 2: shl18_00_fall */
const Sint32 pl18_effect_02[] = {
	CG_FRAME(   0,    0,   2,           0, 32778, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0, 32779, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0, 32780, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0, 32781, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0, 32782, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0, 32783, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0, 32784, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32785, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl18_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl18_effect_03[] = {
	CG_FRAME(   0,    0,   1,           0, 32788, 0x40,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  66 ),
	CG_FRAME(   0,    0,   1,           0, 32792, 0x40,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  66 ),
	CG_FRAME(   0,    0,   1,           0, 32789, 0x40,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  66 ),
	CG_FRAME(   0,    0,   1,           0, 32793, 0x40,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  66 ),
	CG_FRAME(   0,    0,   1,           0, 32790, 0x40,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  66 ),
	CG_FRAME(   0,    0,   1,           0, 32794, 0x40,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  66 ),
	CG_FRAME(   0,    0,   1,           0, 32791, 0x40,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  66 ),
	CG_FRAME(   0,    0,   1, END_OF_DATA, 32795, 0x40,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  66 ),
	CG_NEXT( pl18_effect_03 ),
};

/* Char_Effect 4: shl18_04_init (4 + sub_no) */
const Sint32 pl18_effect_04[] = {
	CG_FRAME(   0,    0,   1, END_OF_DATA,    -1, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl18_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl18_effect_05[] = {
	CG_FRAME(   0,    0,   1, END_OF_DATA,    -1, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl18_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl18_effect_06[] = {
	CG_FRAME(   0,    0,   1, END_OF_DATA,    -1, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl18_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl18_effect_07[] = {
	CG_FRAME(   0,    0,   2,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0, 32779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(   0,    0,   2,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(   0,    0,   2,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(   0,    0,   2,           0, 32782, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0, 32783, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0, 32784, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32785, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl18_effect_07 ),
};

/* Char_Change 0: act_change_tag_init
   Char_Change 1: act_change_vcounter_init
   Char_Change 2: act_change_out_bound
   Char_Change 3: act_stand, act_change_call
   Char_Change 4: act_change_assist_end
   Char_Change 5 */
const Sint32 pl18_change_00[] = {
	CG_FRAME(   0,    0,   3, END_OF_DATA,    38, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl18_change_00 ),
};

/* group 25 0
   Char_Fly 0 */
const Sint32 pl18_grp25_00[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   379, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl18_grp25_00 ),
};

/* group 25 1
   Char_Fly 1: act_fly */
const Sint32 pl18_grp25_01[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA, 32796, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl18_grp25_01 ),
};

/* group 25 2
   Char_Fly 2 */
const Sint32 pl18_grp25_02[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   378, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl18_grp25_02 ),
};

/* group 25 3
   Char_Fly 3 */
const Sint32 pl18_grp25_03[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   382, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl18_grp25_03 ),
};

/* group 25 4
   Char_Fly 4 */
const Sint32 pl18_grp25_04[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA, 32797, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl18_grp25_04 ),
};

/* group 25 5
   Char_Fly 5 */
const Sint32 pl18_grp25_05[] = {
	CG_FRAME(   0,    0,   2, END_OF_DATA,   381, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl18_grp25_05 ),
};

/* group 25 6
   Char_Fly 6 */
const Sint32 pl18_grp25_06[] = {
	CG_FRAME(   0,    0,   2, END_OF_DATA,   385, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl18_grp25_06 ),
};

/* group 25 7
   Char_Fly 7 */
const Sint32 pl18_grp25_07[] = {
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32798, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl18_grp25_07 ),
};

/* group 25 8
   Char_Fly 8 */
const Sint32 pl18_grp25_08[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   384, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl18_grp25_08 ),
};

/* group 25 9
   Char_Fly 9 */
const Sint32 pl18_grp25_09[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   388, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl18_grp25_09 ),
};

/* group 25 10
   Char_Fly 10 */
const Sint32 pl18_grp25_10[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA, 32799, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl18_grp25_10 ),
};

/* group 25 11
   Char_Fly 11 */
const Sint32 pl18_grp25_11[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   387, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl18_grp25_11 ),
};

/* group 25 12
   Char_Fly 12 */
const Sint32 pl18_grp25_12[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA, 32800, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl18_grp25_12 ),
};

/* group 25 13
   Char_Fly 13 */
const Sint32 pl18_grp25_13[] = {
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32801, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl18_grp25_13 ),
};

/* group 25 14
   Char_Fly 14 */
const Sint32 pl18_grp25_14[] = {
	CG_FRAME(   0,    0,   2, END_OF_DATA,   392, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl18_grp25_14 ),
};

/* group 25 15
   Char_Fly 15 */
const Sint32 pl18_grp25_15[] = {
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32802, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl18_grp25_15 ),
};

/* group 25 16
   Char_Fly 16 */
const Sint32 pl18_grp25_16[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA, 32803, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl18_grp25_16 ),
};

/* group 25 17
   Char_Fly 17 */
const Sint32 pl18_grp25_17[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   395, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl18_grp25_17 ),
};

/* group 25 18
   Char_Fly 18 */
const Sint32 pl18_grp25_18[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA, 32804, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl18_grp25_18 ),
};

/* group 25 19
   Char_Fly 19 */
const Sint32 pl18_grp25_19[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA, 32805, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl18_grp25_19 ),
};

/* group 25 20
   Char_Fly 20 */
const Sint32 pl18_grp25_20[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   398, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl18_grp25_20 ),
};

/* group 25 21
   Char_Fly 21 */
const Sint32 pl18_grp25_21[] = {
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32804, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl18_grp25_21 ),
};

/* group 25 22
   Char_Fly 22 */
const Sint32 pl18_grp25_22[] = {
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32805, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl18_grp25_22 ),
};

/* group 25 23
   Char_Fly 23 */
const Sint32 pl18_grp25_23[] = {
	CG_FRAME(   0,    0,   2, END_OF_DATA,   398, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl18_grp25_23 ),
};

/* group 27 0
   group 27 15 */
const Sint32 pl18_grp27_00[] = {
	EF_FRAME(   0,    0,   4,           0,   137 ),
	EF_FRAME(   0,    0,   4,           0,   138 ),
	EF_FRAME(   0,    0,   4,           0,   139 ),
	EF_FRAME(   0,    0,   4,           0,   140 ),
	EF_FRAME(   0,    0,   4,           0,   141 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,    -1 ),
	CG_NEXT( pl18_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl18_grp27_01[] = {
	EF_FRAME(   0,    0,   4,           0,   290 ),
	EF_FRAME(   0,    0,   4,           0,   291 ),
	EF_FRAME(   0,    0,   4,           0,   292 ),
	EF_FRAME(   0,    0,   4,           0,   293 ),
	EF_FRAME(   0,    0,   4,           0,   294 ),
	EF_FRAME(   0,    0,   4,           0,   295 ),
	EF_FRAME(   0,    0,   4,           0,   296 ),
	EF_FRAME(   0,    0,   4,           0,   297 ),
	EF_FRAME(   0,    0,   4,           0,   298 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,   299 ),
	CG_NEXT( pl18_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl18_grp27_02[] = {
	EF_FRAME(   0,    0,   4,           0,   300 ),
	EF_FRAME(   0,    0,   4,           0,   301 ),
	EF_FRAME(   0,    0,   4,           0,   302 ),
	EF_FRAME(   0,    0,   4,           0,   303 ),
	EF_FRAME(   0,    0,   4,           0,   304 ),
	EF_FRAME(   0,    0,   4,           0,   305 ),
	EF_FRAME(   0,    0,   4,           0,   306 ),
	EF_FRAME(   0,    0,   4,           0,   307 ),
	EF_FRAME(   0,    0,   4,           0,   308 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,   309 ),
	CG_NEXT( pl18_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl18_grp27_03[] = {
	EF_FRAME(   0,    0,   4,           0,   310 ),
	EF_FRAME(   0,    0,   4,           0,   311 ),
	EF_FRAME(   0,    0,   4,           0,   312 ),
	EF_FRAME(   0,    0,   4,           0,   313 ),
	EF_FRAME(   0,    0,   4,           0,   314 ),
	EF_FRAME(   0,    0,   4,           0,   315 ),
	EF_FRAME(   0,    0,   4,           0,   316 ),
	EF_FRAME(   0,    0,   4,           0,   317 ),
	EF_FRAME(   0,    0,   4,           0,   318 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,   319 ),
	CG_NEXT( pl18_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl18_grp27_04[] = {
	EF_FRAME(   0,    0,   1,           0,   320 ),
	EF_FRAME(   0,    0,   1,           0,   321 ),
	EF_FRAME(   0,    0,   1,           0,   322 ),
	EF_FRAME(   0,    0,   1,           0,   323 ),
	EF_FRAME(   0,    0,   1,           0,   324 ),
	EF_FRAME(   0,    0,   1,           0,   325 ),
	EF_FRAME(   0,    0,   1,           0,   326 ),
	EF_FRAME(   0,    0,   1,           0,   327 ),
	EF_FRAME(   0,    0,   1,           0,   328 ),
	EF_FRAME(   0,    0,   4,           0,   329 ),
	EF_FRAME(   0,    0,   4,           0,   330 ),
	EF_FRAME(   0,    0,   4,           0,   331 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,   332 ),
	CG_NEXT( pl18_grp27_04 ),
};

/* group 27 5 */
const Sint32 pl18_grp27_05[] = {
	EF_FRAME(   0,    0,   4,           0,   452 ),
	EF_FRAME(   0,    0,   4,           0,   453 ),
	EF_FRAME(   0,    0,   4,           0,   454 ),
	EF_FRAME(   0,    0,   4,           0,   455 ),
	EF_FRAME(   0,    0,   4,           0,   456 ),
};

const Sint32 pl18_grp27_05_loop[] = {
	EF_FRAME(   0,    0,   4,           0,   457 ),
	EF_FRAME(   0,    0,   4,           0,   458 ),
	EF_FRAME(   0,    0,   4,           0,   459 ),
	EF_FRAME(   0,    0,   4,           0,   460 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,   461 ),
	CG_NEXT( pl18_grp27_05_loop ),
};

/* group 27 6
   group 27 8 */
const Sint32 pl18_grp27_06[] = {
	EF_FRAME(   0,    0,   1,           0,   356 ),
	EF_FRAME(   0,    0,   1,           0,   357 ),
	EF_FRAME(   0,    0,   1,           0,   358 ),
	EF_FRAME(   0,    0,   1, END_OF_DATA,   359 ),
	CG_NEXT( pl18_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl18_grp27_07[] = {
	EF_FRAME(   0,    0,   1,           0,   128 ),
	EF_FRAME(   0,    0,   1, END_OF_DATA,    -1 ),
	CG_NEXT( pl18_grp27_07 ),
};

/* group 27 9 */
const Sint32 pl18_grp27_09[] = {
	EF_FRAME(   0,    0,   1,           0,   185 ),
	EF_FRAME(   0,    0,   4,           0,   186 ),
	EF_FRAME(   0,    0,   4,           0,   187 ),
	EF_FRAME(   0,    0,   4,           0,   188 ),
	EF_FRAME(   0,    0,   4,           0,   189 ),
	EF_FRAME(   0,    0,   4,           0,   190 ),
	EF_FRAME(   0,    0,   4,           0,   191 ),
	EF_FRAME(   0,    0,   4,           0,   192 ),
	EF_FRAME(   0,    0,   4,           0,   193 ),
	EF_FRAME(   0,    0,   4,           0,   194 ),
	EF_FRAME(   0,    0,   4,           0,   195 ),
	EF_FRAME(   0,    0,   4,           0,   196 ),
	EF_FRAME(   0,    0,   4,           0,   197 ),
	EF_FRAME(   0,    0,   4,           0,   198 ),
	EF_FRAME(   0,    0,   4,           0,   199 ),
	EF_FRAME(   0,    0,   4,           0,   200 ),
	EF_FRAME(   0,    0,   4,           0,   201 ),
	EF_FRAME(   0,    0,   4,           0,   202 ),
	EF_FRAME(   0,    0,   4,           0,   203 ),
	EF_FRAME(   0,    0,   4,           0,   204 ),
	EF_FRAME(   0,    0,   4,           0,   205 ),
	EF_FRAME(   0,    0,   4,           0,   206 ),
	EF_FRAME(   0,    0,   4,           0,   207 ),
	EF_FRAME(   0,    0,   4,           0,   208 ),
	EF_FRAME(   0,    0,   4,           0,   209 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,   210 ),
	CG_NEXT( pl18_grp27_09 ),
};

/* group 27 10 */
const Sint32 pl18_grp27_10[] = {
	EF_FRAME(   0,    0,   2,           0,   427 ),
	EF_FRAME(   0,    0,   2,           0,   428 ),
	EF_FRAME(   0,    0,   2,           0,   429 ),
	EF_FRAME(   0,    0,   2,           0,   430 ),
	EF_FRAME(   0,    0,   2,           0,   431 ),
	EF_FRAME(   0,    0,   2,           0,   432 ),
	EF_FRAME(   0,    0,   2,           0,   433 ),
	EF_FRAME(   0,    0,   2,           0,   434 ),
	EF_FRAME(   0,    0,   2,           0,   435 ),
	EF_FRAME(   0,    0,   2,           0,   436 ),
	EF_FRAME(   0,    0,   2,           0,   437 ),
	EF_FRAME(   0,    0,   2,           0,   438 ),
	EF_FRAME(   0,    0,   2,           0,   439 ),
	EF_FRAME(   0,    0,   2,           0,   440 ),
	EF_FRAME(   0,    0,   2,           0,   441 ),
	EF_FRAME(   0,    0,   2,           0,   442 ),
	EF_FRAME(   0,    0,   2,           0,   443 ),
	EF_FRAME(   0,    0,   2,           0,   444 ),
	EF_FRAME(   0,    0,   2,           0,   445 ),
	EF_FRAME(   0,    0,   2,           0,   446 ),
	EF_FRAME(   0,    0,   2,           0,   447 ),
	EF_FRAME(   0,    0,   2,           0,   448 ),
	EF_FRAME(   0,    0,   2,           0,   449 ),
	EF_FRAME(   0,    0,   2,           0,   450 ),
	EF_FRAME(   0,    0,   2, END_OF_DATA,   451 ),
	CG_NEXT( pl18_grp27_10 ),
};

/* group 27 11 */
const Sint32 pl18_grp27_11[] = {
	EF_FRAME(   0,    0,   3,           0,   409 ),
	EF_FRAME(   0,    0,   3,           0,   410 ),
	EF_FRAME(   0,    0,   3,           0,   411 ),
	EF_FRAME(   0,    0,   3,           0,   412 ),
	EF_FRAME(   0,    0,   3,           0,   413 ),
	EF_FRAME(   0,    0,   3,           0,   414 ),
	EF_FRAME(   0,    0,   3,           0,   415 ),
	EF_FRAME(   0,    0,   3,           0,   416 ),
	EF_FRAME(   0,    0,   3, END_OF_DATA,   417 ),
	CG_NEXT( pl18_grp27_11 ),
};

/* group 27 12 */
const Sint32 pl18_grp27_12[] = {
	EF_FRAME(   0,    0,  10,           0,    -1 ),
	EF_FRAME(   0,    0,   1,           0,   418 ),
	EF_FRAME(   0,    0,   1,           0,   419 ),
	EF_FRAME(   0,    0,   1,           0,   420 ),
	EF_FRAME(   0,    0,   1,           0,   421 ),
	EF_FRAME(   0,    0,   1,           0,   422 ),
	EF_FRAME(   0,    0,   1,           0,   423 ),
	EF_FRAME(   0,    0,   1,           0,   424 ),
	EF_FRAME(   0,    0,   1,           0,   423 ),
	EF_FRAME(   0,    0,   1,           0,   424 ),
	EF_FRAME(   0,    0,   1,           0,   425 ),
	EF_FRAME(   0,    0,   1,           0,   426 ),
	EF_FRAME(   0,    0,   1,           0,   425 ),
	EF_FRAME(   0,    0,   1,           0,   426 ),
	EF_FRAME(   0,    0,   1,           0,   360 ),
	EF_FRAME(   0,    0,   1,           0,   370 ),
	EF_FRAME(   0,    0,   1,           0,   361 ),
	EF_FRAME(   0,    0,   1,           0,   371 ),
	EF_FRAME(   0,    0,   1,           0,   362 ),
	EF_FRAME(   0,    0,   1,           0,   372 ),
	EF_FRAME(   0,    0,   1,           0,   363 ),
	EF_FRAME(   0,    0,   1,           0,   373 ),
	EF_FRAME(   0,    0,   1,           0,   364 ),
	EF_FRAME(   0,    0,   1,           0,   374 ),
	EF_FRAME(   0,    0,   1,           0,   365 ),
	EF_FRAME(   0,    0,   1,           0,   375 ),
	EF_FRAME(   0,    0,   1,           0,   366 ),
	EF_FRAME(   0,    0,   1,           0,   376 ),
	EF_FRAME(   0,    0,   1,           0,   367 ),
	EF_FRAME(   0,    0,   1,           0,   377 ),
	EF_FRAME(   0,    0,   1,           0,   368 ),
	EF_FRAME(   0,    0,   1, END_OF_DATA,   369 ),
	CG_NEXT( pl18_grp27_12 ),
};

/* group 27 13 */
const Sint32 pl18_grp27_13[] = {
	EF_FRAME(   0,    0,   4,           0,   399 ),
	EF_FRAME(   0,    0,   4,           0,   400 ),
	EF_FRAME(   0,    0,   4,           0,   401 ),
	EF_FRAME(   0,    0,   4,           0,   402 ),
	EF_FRAME(   0,    0,   4,           0,   403 ),
	EF_FRAME(   0,    0,   4,           0,   404 ),
	EF_FRAME(   0,    0,   4,           0,   405 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,   406 ),
	CG_NEXT( pl18_grp27_13 ),
};

/* group 27 14 */
const Sint32 pl18_grp27_14[] = {
	EF_FRAME(   0,    0,   2,           0,   287 ),
	EF_FRAME(   0,    0,   2,           0,   288 ),
	EF_FRAME(   0,    0,   2, END_OF_DATA,   289 ),
	CG_NEXT( pl18_grp27_14 ),
};

/* group 27 16 */
const Sint32 pl18_grp27_16[] = {
	EF_FRAME(   0,    0,   4,           0,   246 ),
	EF_FRAME(   0,    0,   4,           0,   219 ),
};

/* group 27 17 */
const Sint32 pl18_grp27_17[] = {
	EF_FRAME(   0,    0,   4,           0,   247 ),
	EF_FRAME(   0,    0,   4,           0,   220 ),
};

/* group 27 18 */
const Sint32 pl18_grp27_18[] = {
	EF_FRAME(   0,    0,   4,           0,   248 ),
	EF_FRAME(   0,    0,   4,           0,   221 ),
};

/* group 27 19 */
const Sint32 pl18_grp27_19[] = {
	EF_FRAME(   0,    0,   4,           0,   249 ),
	EF_FRAME(   0,    0,   4,           0,   222 ),
};

/* group 27 20 */
const Sint32 pl18_grp27_20[] = {
	EF_FRAME(   0,    0,   4,           0,   250 ),
	EF_FRAME(   0,    0,   4,           0,   223 ),
};

/* group 27 21 */
const Sint32 pl18_grp27_21[] = {
	EF_FRAME(   0,    0,   4,           0,   251 ),
	EF_FRAME(   0,    0,   4,           0,   224 ),
};

/* group 27 22 */
const Sint32 pl18_grp27_22[] = {
	EF_FRAME(   0,    0,   4,           0,   252 ),
	EF_FRAME(   0,    0,   4,           0,   225 ),
};

/* group 27 23 */
const Sint32 pl18_grp27_23[] = {
	EF_FRAME(   0,    0,   4,           0,   253 ),
	EF_FRAME(   0,    0,   4,           0,   226 ),
};

/* group 27 24 */
const Sint32 pl18_grp27_24[] = {
	EF_FRAME(   0,    0,   4,           0,   254 ),
	EF_FRAME(   0,    0,   4,           0,   227 ),
};

/* group 27 25 */
const Sint32 pl18_grp27_25[] = {
	EF_FRAME(   0,    0,   4,           0,   255 ),
	EF_FRAME(   0,    0,   4,           0,   228 ),
};

/* group 27 26 */
const Sint32 pl18_grp27_26[] = {
	EF_FRAME(   0,    0,   4,           0,   256 ),
	EF_FRAME(   0,    0,   4,           0,   229 ),
};

/* group 27 27 */
const Sint32 pl18_grp27_27[] = {
	EF_FRAME(   0,    0,   4,           0,   257 ),
	EF_FRAME(   0,    0,   4,           0,   230 ),
};

/* group 27 28 */
const Sint32 pl18_grp27_28[] = {
	EF_FRAME(   0,    0,   4,           0,   258 ),
	EF_FRAME(   0,    0,   4,           0,   231 ),
};

/* group 27 29 */
const Sint32 pl18_grp27_29[] = {
	EF_FRAME(   0,    0,   4,           0,   259 ),
	EF_FRAME(   0,    0,   4,           0,   232 ),
};

/* group 27 30 */
const Sint32 pl18_grp27_30[] = {
	EF_FRAME(   0,    0,   4,           0,   260 ),
	EF_FRAME(   0,    0,   4,           0,   233 ),
};

/* group 27 31 */
const Sint32 pl18_grp27_31[] = {
	EF_FRAME(   0,    0,   4,           0,   261 ),
	EF_FRAME(   0,    0,   4,           0,   234 ),
};

/* group 27 32 */
const Sint32 pl18_grp27_32[] = {
	EF_FRAME(   0,    0,   4,           0,   262 ),
	EF_FRAME(   0,    0,   4,           0,   235 ),
};

/* group 27 33 */
const Sint32 pl18_grp27_33[] = {
	EF_FRAME(   0,    0,   4,           0,   263 ),
	EF_FRAME(   0,    0,   4,           0,   236 ),
};

/* group 27 34 */
const Sint32 pl18_grp27_34[] = {
	EF_FRAME(   0,    0,   4,           0,   264 ),
	EF_FRAME(   0,    0,   4,           0,   237 ),
};

/* group 27 35 */
const Sint32 pl18_grp27_35[] = {
	EF_FRAME(   0,    0,   4,           0,   265 ),
	EF_FRAME(   0,    0,   4,           0,   238 ),
};

/* group 27 36 */
const Sint32 pl18_grp27_36[] = {
	EF_FRAME(   0,    0,   4,           0,   266 ),
	EF_FRAME(   0,    0,   4,           0,   239 ),
};

/* group 27 37 */
const Sint32 pl18_grp27_37[] = {
	EF_FRAME(   0,    0,   4,           0,   267 ),
	EF_FRAME(   0,    0,   4,           0,   240 ),
};

/* group 27 38 */
const Sint32 pl18_grp27_38[] = {
	EF_FRAME(   0,    0,   4,           0,   268 ),
	EF_FRAME(   0,    0,   4,           0,   241 ),
};

/* group 27 39 */
const Sint32 pl18_grp27_39[] = {
	EF_FRAME(   0,    0,   4,           0,   269 ),
	EF_FRAME(   0,    0,   4,           0,   242 ),
};

/* group 27 40 */
const Sint32 pl18_grp27_40[] = {
	EF_FRAME(   0,    0,   4,           0,   270 ),
	EF_FRAME(   0,    0,   4,           0,   243 ),
};

/* group 27 41 */
const Sint32 pl18_grp27_41[] = {
	EF_FRAME(   0,    0,   4,           0,   271 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,   244 ),
	CG_NEXT( pl18_grp27_16 ),
};

/* group 27 42 */
const Sint32 pl18_grp27_42[] = {
	EF_FRAME(   0,    0,   1,           0,   462 ),
	EF_FRAME(   0,    0,   1,           0,   463 ),
	EF_FRAME(   0,    0,   1,           0,   464 ),
	EF_FRAME(   0,    0,   1,           0,   465 ),
	EF_FRAME(   0,    0,   1,           0,   466 ),
	EF_FRAME(   0,    0,   1,           0,   467 ),
	EF_FRAME(   0,    0,   1,           0,   468 ),
	EF_FRAME(   0,    0,   1,           0,   469 ),
	EF_FRAME(   0,    0,   1,           0,   470 ),
	EF_FRAME(   0,    0,   1,           0,   471 ),
	EF_FRAME(   0,    0,   1,           0,   472 ),
	EF_FRAME(   0,    0,   1, END_OF_DATA,   473 ),
	CG_NEXT( pl18_grp27_42 ),
};

/* group 27 47 */
const Sint32 pl18_grp27_47[] = {
	EF_FRAME(   0,    0,   1,           0,   474 ),
	EF_FRAME(   0,    0,   1,           0,   475 ),
	EF_FRAME(   0,    0,   1,           0,   476 ),
	EF_FRAME(   0,    0,   1,           0,   477 ),
	EF_FRAME(   0,    0,   1,           0,   478 ),
	EF_FRAME(   0,    0,   1,           0,   479 ),
	EF_FRAME(   0,    0,   1,           0,   480 ),
	EF_FRAME(   0,    0,   1,           0,   481 ),
	EF_FRAME(   0,    0,   1,           0,   482 ),
	EF_FRAME(   0,    0,   1,           0,   483 ),
	EF_FRAME(   0,    0,   1,           0,   484 ),
	EF_FRAME(   0,    0,   1,           0,   485 ),
	EF_FRAME(   0,    0,   1,           0,   486 ),
	EF_FRAME(   0,    0,   1,           0,   487 ),
	EF_FRAME(   0,    0,   1,           0,   488 ),
	EF_FRAME(   0,    0,   1, END_OF_DATA,   489 ),
	CG_NEXT( pl18_grp27_47 ),
};

/* group 27 43 */
const Sint32 pl18_grp27_43[] = {
	EF_FRAME(   0,    0,   4, END_OF_DATA,   217 ),
	CG_NEXT( pl18_grp27_43 ),
};

/* group 27 44 */
const Sint32 pl18_grp27_44[] = {
	EF_FRAME(   0,    0,   2,           0,   333 ),
	EF_FRAME(   0,    1,   2,           0,   334 ),
	EF_FRAME(   1,    1,   2,           0,   335 ),
	EF_FRAME(   2,    1,   2,           0,   336 ),
	EF_FRAME(   3,    1,   2,           0,   337 ),
	EF_FRAME(   0,    0,   2,           0,   338 ),
	EF_FRAME(   0,    0,   2, END_OF_DATA,   339 ),
	CG_NEXT( pl18_grp27_44 ),
};

/* group 27 45
   group 27 46 */
const Sint32 pl18_grp27_45[] = {
	EF_FRAME(   0,    0,   2,           0,   340 ),
	EF_FRAME(   0,    0,   2,           0,   341 ),
	EF_FRAME(   0,    0,   2,           0,   342 ),
	EF_FRAME(   0,    0,   2, END_OF_DATA,   343 ),
	CG_NEXT( pl18_grp27_45 ),
};

/* group 27 48 */
const Sint32 pl18_grp27_48[] = {
	EF_FRAME(   0,    0,   4,           0,   407 ),
	EF_FRAME(   0,    0,   4,           0,   408 ),
	EF_FRAME(   0,    0,   4, END_OF_DATA,    -1 ),
	CG_NEXT( pl18_grp27_48 ),
};
