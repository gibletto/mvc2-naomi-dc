/* DM0C, the memory card window's message: its lines by vms_work.vm_msg[0], with the points earned and
   the card's port drawn in, gone when the message changes. */
#include "player.h"
#include "nl.h"

void vm_msg_move( PLAYER_WORK *w ) ;
void vm_msg_init( PLAYER_WORK *w ) ;
void vm_msg_wait( PLAYER_WORK *w ) ;
void vm_msg_end( PLAYER_WORK *w ) ;
PLAYER_WORK *vm_msg_sub_set( int n, int code, VEC3 *pos ) ;
void vm_msg_num_set( PLAYER_WORK *w, int num, int keta, int col, VEC3 *pos ) ;

static Float vm_msg_col[4][3] = {	/* white, then the colours of the two numbers and the port */
	{ 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }
} ;

/* each message's lines: a model past the window's own in the low byte, what to draw in the high
   byte, -1 at the end */
static Sint16 vm_msg_0[3] = { 0x100, 2, -1 } ;
static Sint16 vm_msg_1[3] = { 0x201, 2, -1 } ;
static Sint16 vm_msg_2[4] = { 0x300, 1, 2, -1 } ;
static Sint16 vm_msg_3[2] = { 0x50d, -1 } ;
static Sint16 vm_msg_4[2] = { 0x0e, -1 } ;
static Sint16 vm_msg_5[2] = { 0x0f, -1 } ;
static Sint16 vm_msg_6[2] = { 0x410, -1 } ;
static Sint16 *vm_msg_tbl[8] = {
	vm_msg_0, vm_msg_0, vm_msg_1, vm_msg_2, vm_msg_3, vm_msg_4, vm_msg_5, vm_msg_6
} ;

static VEC3 vm_msg_pos_0[2] = { { 2.0f, -0.8f, -13.0f }, { 0.0f, 0.7f, -13.0f } } ;
static VEC3 vm_msg_pos_1[3] = { { 2.0f, 1.3f, -13.0f }, { 2.0f, 0.0f, -13.0f }, { 0.0f, -1.5f, -13.0f } } ;
static VEC3 vm_msg_pos_2[1] = { { 0.0f, 0.0f, 0.0f } } ;
static VEC3 *vm_msg_pos_tbl[8] = {
	vm_msg_pos_0, vm_msg_pos_0, vm_msg_pos_0, vm_msg_pos_1,
	vm_msg_pos_2, vm_msg_pos_2, vm_msg_pos_2, vm_msg_pos_2
} ;

static VEC3 vm_msg_num_pos[16] = {	/* the digits of each line's number, and the port's letter and number */
	{ -3.0f, -0.8f, -13.0f }, { -2.0f, -0.8f, -13.0f }, { -1.0f, -0.8f, -13.0f }, { 0.0f, -0.8f, -13.0f },
	{ -3.0f, 1.3f, -13.0f }, { -2.0f, 1.3f, -13.0f }, { -1.0f, 1.3f, -13.0f }, { 0.0f, 1.3f, -13.0f },
	{ -3.0f, 0.0f, -13.0f }, { -2.0f, 0.0f, -13.0f }, { -1.0f, 0.0f, -13.0f }, { 0.0f, 0.0f, -13.0f },
	{ -7.1f, -0.24f, -13.0f }, { -6.0f, -0.24f, -13.0f }, { 3.7f, -2.0f, -13.0f }, { 4.8f, -2.0f, -13.0f }
} ;
static Uint8 vm_msg_num_idx[6] = { 0, 0, 0, 4, 12, 14 } ;	/* the first of vm_msg_num_pos by what is drawn */
static void ( *vm_msg_jmp[3] )( PLAYER_WORK *w ) = { vm_msg_init, vm_msg_wait, vm_msg_end } ;

/****************************************************************/
/*			The card message's lines							*/
/****************************************************************/
void dm0c_vm_msg_set( void )
{
	Sint16 *p ;
	VEC3 *pos ;
	int n ;

	n = vms_work.vm_msg[0] ;
	p = vm_msg_tbl[n] ;
	pos = vm_msg_pos_tbl[n] ;
	while( *p != -1 ) vm_msg_sub_set( n, *p ++, pos ++ ) ;
}

/****************************************************************/
/*			A line each frame									*/
/****************************************************************/
void vm_msg_move( PLAYER_WORK *w )
{
	vm_msg_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			What the line draws in: the points or the port		*/
/****************************************************************/
void vm_msg_init( PLAYER_WORK *w )
{
	PLAYER_WORK *w2 ;
	VEC3 *pos ;
	VMS_WORK *vm ;

	vm = &vms_work ;
	w->routine_0 ++ ;
	pos = &vm_msg_num_pos[vm_msg_num_idx[w->timer0 >> 8]] ;
	switch( w->timer0 ){
	case 0x300 :
	case 0x100 :
		vm_msg_num_set( w, vm->d_point, 4, 1, pos ) ;
		if( w->timer0 == 0x100 ) break ;
		pos += 4 ;
	case 0x200 :
		vm_msg_num_set( w, vm->n_point, 4, 2, pos ) ;
		break ;
	case 0x400 :
	case 0x500 :
		w2 = vm_msg_sub_set( w->type, vm->file_port / 2 + 17, pos ++ ) ;
		if( w2 ) w2->sub_type = 3 ;
		w2 = vm_msg_sub_set( w->type, ( vm->file_port & 1 ) + 4, pos ) ;
		if( w2 ) w2->sub_type = 3 ;
		break ;
	}
	vm_msg_wait( w ) ;
}

/****************************************************************/
/*			Lit in its colour until the message changes			*/
/****************************************************************/
void vm_msg_wait( PLAYER_WORK *w )
{
	Float *c ;

	if( vms_work.vm_msg[0] != w->type ){
		w->routine_0 ++ ;
		w->disp_flag = 0 ;
		return ;
	}
	c = vm_msg_col[w->sub_type] ;
	w->fade.r = c[0] ;
	w->fade.g = c[1] ;
	w->fade.b = c[2] ;
}

void vm_msg_end( PLAYER_WORK *w )
{
	work_free( w ) ;
}

/****************************************************************/
/*			A line of the message, or a character drawn in		*/
/****************************************************************/
PLAYER_WORK *vm_msg_sub_set( int n, int code, VEC3 *pos )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return NULL ;
	w->disp_flag = 1 ;
	w->work_func = vm_msg_move ;
	w->obj = obj_dm0c->obj[( code & ~0xff00 ) + 318] ;
	w->obj_flag = OBJ_POS | OBJ_FADE ;
	w->pos = *pos ;
	w->type = n ;
	w->sub_type = 0 ;
	w->timer0 = code & 0xff00 ;
	return w ;
}

/****************************************************************/
/*			A number of keta digits, from its first that isn't	*/
/*			0													*/
/****************************************************************/
void vm_msg_num_set( PLAYER_WORK *w, int num, int keta, int col, VEC3 *pos )
{
	PLAYER_WORK *w2 ;
	int i, n, f ;

	f = 0 ;
	for( i = 0 ; i < keta ; i ++, pos ++ ){
		n = num / ( int )nlPow( 10.0f, ( Float )( keta - i - 1 ) ) ;
		n %= 10 ;
		if( i == keta - 1 ) f = 1 ;
		if( f || n ){
			f = 1 ;
			if( ( w2 = vm_msg_sub_set( w->type, n + 3, pos ) ) ) w2->sub_type = col ;
		}
	}
}
