/* Three small routines of the select scene's kind 5 works, after the cells: a do-nothing, hide the work, hide and free it. */
#include "player.h"

extern void sel_dummy_1( PLAYER_WORK *w );
void sel_hide( PLAYER_WORK *ew );
void sel_hide_free( PLAYER_WORK *ew );

/* six places, by side and slot (never used) */
static VEC3 sel_tail_pos_tbl[6] = {
	{ 4.0f, 78.0f, 19.9f }, { 8.0f, 79.5f, 19.9f }, { -8.0f, 82.5f, 19.9f },
	{ -4.0f, 78.0f, 19.9f }, { -8.0f, 79.5f, 19.9f }, { 8.0f, 82.5f, 19.9f },
} ;

/* each character's assist type to start with, by character */
Sint8 sel_personal_tbl[60] = {
	1, 1, 2, 2, 0, 0, 0, 0, 2, 2, 1, 2, 0, 1, 0, 2, 0, 0, 0, 1,
	0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 2, 1, 1, 2, 0, 2, 0, 1, 1, 0,
	1, 1, 0, 0, 1, 2, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 1, 2, 2, 0,
} ;

/* a work's steps with the routines below (never used) */
static void ( *sel_tail_jmp[] )( PLAYER_WORK *ew ) = {
	sel_dummy_1, sel_dummy_1, sel_dummy_1, sel_hide, sel_hide, sel_hide_free,
} ;

/****************************************************************/
void sel_nop( PLAYER_WORK *ew )
{
}

/****************************************************************/
void sel_hide( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
}

/****************************************************************/
void sel_hide_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_tail.c" End of File	*/
