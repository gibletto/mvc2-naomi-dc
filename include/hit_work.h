/* The works: the pool they come from, their lists and draw priorities (hit_work.c), and the hit
   check (hit.c). player.h includes it. */
#ifndef HIT_WORK_H
#define HIT_WORK_H

#define HIT_W_MAX	94	/* works each side can enter for the hit check */

/* hit_work.c: the works and their lists */
#define WORK_MAX	256	/* works in the pool */
#define WORK_SIZE	0x1d0	/* each one's size: an effect's work ends where a player's act_st starts */
typedef struct { Uint8 buff[WORK_SIZE]; } WORK_BUFF;	/* one work of the pool */
#define LIST_MAX	14	/* lists of works */
/* the lists, a work's work_kind */
#define W_PL		0	/* a player: not from the pool */
#define W_SHL		1	/* shots (shlXX_YY) */
#define W_SHL_ST	2	/* shots that go on while a hyper combo stops the game */
#define W_PLS		3	/* player effects (plsXX_YY) */
#define W_PLS_ST	4	/* player effects that go on through the stop */
#define W_SET		5	/* the stage's objects (setXX_YY) and the menus' pieces */
#define W_BACK		6	/* shadows, the hyper combo's background */
#define W_EF		7	/* dust, sparks, guard and damage effects */
#define W_MARK		8	/* clash marks, hyper sparks and waves */
#define W_FINISH	9	/* the finish (finish_ef) */
#define W_DM		10	/* damage effects (dmXX_YY) */
#define W_SYS		11	/* gauges, messages, the scenes' own pieces */
#define W_FRONT		12	/* in front of everything */
#define W_PAUSE		13	/* the pause's menus (disp_pause) */
/* work_get()'s mode: where the work joins its list */
#define LIST_TOP	0
#define LIST_END	1
#define LIST_BEHIND	2	/* after base */
#define LIST_BEFORE	3	/* before base */
#define PRIO_MAX	16	/* draw priorities */
#define PRIO_W_MAX	96	/* works drawn at each priority */
extern void work_init( void );
extern void work_list_kill( Uint8 list );
extern void work_list_move( Uint8 list );
extern PLAYER_WORK *work_get( PLAYER_WORK *base, Uint8 list, Uint8 mode );	/* a free work, keeping 10 back */
extern PLAYER_WORK *work_get2( PLAYER_WORK *base, Uint8 list, Uint8 mode );	/* a free work, or 0 */
extern void work_list_disp( Uint8 list );
extern void work_free( PLAYER_WORK *w );	/* gives a work back to the pool */
extern void disp_prio_set( PLAYER_WORK *w );
extern void disp_prio_clr( void );

/* hit.c: the hit check */
extern void hit_ck0( void );
extern void hit_work_init( void );
extern void hit_work_set( PLAYER_WORK *w );	/* enters a work for this frame's hit check */
extern int box_ck( PLAYER_WORK *at_w, PLAYER_WORK *dm_w, Sint16 at_no, Sint16 dm_no );	/* do two boxes overlap? */
extern void all_work_move( void );

#endif
