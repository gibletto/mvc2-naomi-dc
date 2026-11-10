/* Silver Samurai's effect 02 (character 2f's shot 02): eight sets of three pieces that fly out
   from him in the shapes of the table, each blinking for a few frames. */
#include "player.h"
#include "pl_char.h"
#include "pl2f_def.h"

/* a shape: where the piece is from its place, whether it is turned, and its animation */
typedef struct SHL2F_02_SHAPE {
	Sint16	x, y;
	Sint16	flip;
	Sint16	char_no;
} SHL2F_02_SHAPE;

/* the effect's own work */
typedef struct SHL2F_02_LOCAL_W {
	Sint32	char_no;	/* 000: the master's animation when it was set */
	Sint32	x;		/* 004: its place, from the master */
	Sint32	y;		/* 008 */
	Sint32	shape;		/* 00c: the shape it takes */
} SHL2F_02_LOCAL_W;
#define SHL2F_02_LW( w )	( ( SHL2F_02_LOCAL_W * )&( w )->obj_flag )

/* the shapes of three pieces, then where the second is from the first, and where the third is
   from the second */
static Sint16 shl2f_02_set_tbl[8][7] = {
	{ 0, 1, 2, 0, 144, 96, 96 },
	{ 1, 5, 3, 96, 96, 160, 16 },
	{ 6, 4, 0, 144, 0, 144, 64 },
	{ 3, 6, 0, 96, 80, 144, 0 },
	{ 2, 3, 4, 0, 112, 96, 80 },
	{ 5, 2, 1, 160, 16, 0, 112 },
	{ 4, 0, 1, 144, 64, 0, 144 },
	{ 1, 2, 5, 96, 96, 0, 112 },
} ;

static SHL2F_02_SHAPE shl2f_02_shape[] = {
	{ 0, 72, 0, 21 },
	{ 48, 48, 0, 22 },
	{ 0, 56, 0, 23 },
	{ 48, 40, 1, 24 },
	{ 72, 32, 0, 25 },
	{ 80, 8, 0, 26 },
	{ 72, 0, 0, 27 },
	{ 48, 48, 0, 28 },
} ;

static void shl2f_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2f_02_set( PLAYER_WORK *pl_w, Sint16 x, Sint16 y )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2f_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2f02 ;
		SHL2F_02_LW( ew )->char_no = pl_w->char_no.w ;
		SHL2F_02_LW( ew )->x = x ;
		SHL2F_02_LW( ew )->y = y ;
	}
	return ew ;
}

/****************************************************************/
void *shl2f_02_start( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint16 *st ;
	Sint16 i ;

	for( i = 0 ; i < 8 ; i ++ ){
		st = shl2f_02_set_tbl[Rnd() & 7] ;
		if( ( ew = shl2f_02_set( pl_w, 0, 0 ) ) ){
			ew->sub_type = 0 ;
			ew->type = i ;
			SHL2F_02_LW( ew )->shape = st[0] ;
		}
		if( ( ew = shl2f_02_set( pl_w, st[3], st[4] ) ) ){
			ew->sub_type = 1 ;
			ew->type = i ;
			SHL2F_02_LW( ew )->shape = st[1] ;
		}
		if( ( ew = shl2f_02_set( pl_w, st[3] + st[5], st[4] + st[6] ) ) ){
			ew->sub_type = 1 ;
			ew->type = i ;
			SHL2F_02_LW( ew )->shape = st[2] ;
		}
	}
	return ew ;
}

static void shl2f_02_init( PLAYER_WORK *ew );
static void shl2f_02_move( PLAYER_WORK *ew );
static void shl2f_02_end( PLAYER_WORK *ew );
static void shl2f_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl2f_02_main( PLAYER_WORK *ew )
{
	static void ( *shl2f_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl2f_02_init, shl2f_02_move, shl2f_02_end, shl2f_02_free,
	} ;

	shl2f_02_jmp[ew->routine_0]( ew ) ;
}

/* its attack, by chance */
static Uint16 shl2f_02_atck_tbl[] = {
	56, 56, 56, 56, 56, 56, 54, 54, 55, 55, 55, 55, 55, 55, 54, 54,
} ;

/*==============================================================*/
static void shl2f_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL2F_02_LOCAL_W *lw ;
	SHL2F_02_SHAPE *sh ;
	Sint16 x, y, ofs, no ;

	ew->routine_0 ++ ;
	ew->timer0 = 5 ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = ew->type & 1 ;
	ew->prio = 8 ;
	ew->dir = ew->disp_dir = ew->type >> 1 ;
	ew->disp_dir &= 1 ;
	ofs = 16 ;
	if( pl_w->disp_dir ) ofs = -16 ;
	ew->pos.x = pl_w->pos.x + DOT_X( ofs ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 136.0f ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	no = shl2f_02_atck_tbl[Rnd() & 15] ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
	lw = SHL2F_02_LW( ew ) ;
	sh = &shl2f_02_shape[lw->shape] ;
	x = lw->x + sh->x ;
	y = lw->y + sh->y ;
	if( sh->flip ) ew->disp_dir ^= 1 ;
	pl_char_set( ew, 21, sh->char_no + ( ( ew->dir & 2 ) << 2 ) ) ;
	if( ew->dir & 1 ) x = -x ;
	if( ew->dir & 2 ){
		y = -y ;
		ew->disp_dir ^= 1 ;
	}
	ew->pos.x += DOT_X( x ) ;
	ew->pos.y += DOT_Y( y ) ;
	shl2f_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl2f_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag ^= 1 ;
	if( -- ew->timer0 < 0 ) goto end ;
	if( SHL2F_02_LW( ew )->char_no != pl_w->char_no.w ) goto end ;
	pl_char_move( ew ) ;
	if( ew->sub_type || !( Rnd() & 7 ) ) hit_work_set( ew ) ;
	return ;
end:
	shl2f_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl2f_02_end( PLAYER_WORK *ew )
{
}

/*--------------------------------------------------------------*/
static void shl2f_02_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2f_02.c" End of File	*/
