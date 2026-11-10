/* The computer's script helpers: reading a command and its parameter, and the checks a command
   makes before it starts. */
#include "player.h"

/*==============================================================*/
static void cmd_old_set( PLAYER_WORK *pl_w )
{
	pl_w->cpu_cmd_old = pl_w->cpu_cmd ;
}

/****************************************************************/
/*			Reads the command (type 0) or its parameter: a		*/
/*			byte, a word, or a word scaled to a distance		*/
/****************************************************************/
void cmd_read( PLAYER_WORK *pl_w, CPU_SCRIPT *sc, int type )
{
	Uint8 *p = sc->adrs + sc->ofs ;

	switch( type ){
	case 0 :
		cmd_old_set( pl_w ) ;
		pl_w->cpu_cmd = *p ;
		sc->ofs ++ ;
		sc->cmd_cnt ++ ;
		break ;
	case 1 :
		pl_w->cpu_prm.l = *p ;
		sc->ofs += 1 ;
		break ;
	case 2 :
		pl_w->cpu_prm.l = ( Uint16 )( ( Sint32 )( ( *p << 8 | p[1] ) << 16 ) >> 16 ) ;
		sc->ofs += 2 ;
		break ;
	case 3 :
		pl_w->cpu_prm.l = ( Sint32 )( ( Uint16 )( *p << 8 | p[1] ) << 16 ) >> 16 ;
		pl_w->cpu_prm.f = DOT_X( pl_w->cpu_prm.l ) ;
		sc->ofs += 2 ;
		break ;
	case 4 :
		pl_w->cpu_prm.l = ( Sint32 )( ( Uint16 )( *p << 8 | p[1] ) << 16 ) >> 16 ;
		pl_w->cpu_prm.f = DOT_Y( pl_w->cpu_prm.l ) ;
		sc->ofs += 2 ;
		break ;
	}
}

/****************************************************************/
int cmd_act_ck( PLAYER_WORK *pl_w, int no )
{
	return em_act_ck( pl_w, no ) ;
}

/****************************************************************/
/*			Whether a command of kind no can start now			*/
/****************************************************************/
int cmd_start_ck( PLAYER_WORK *pl_w, int no )
{
	switch( no ){
	case 0 :
		if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
		break ;
	case 5 :
		if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
		if( pl_w->cpu_cancel_flag & CC_GUARD ) goto ok ;
		break ;
	case 2 :
		if( !pl_w->cg_atck_flag & CG_ATCK_ALL ) goto ok ;
		break ;
	}
	if( pl_w->cpu_cancel_flag < 0 ) return 1 ;
ok :
	if( !cmd_act_ck( pl_w, no ) ) return 0 ;
	if( pl_w->cpu_cancel_flag ){
		if( pl_w->cpu_cancel_flag & CC_GUARD ){
			pl_w->cpu_cancel_flag ^= CC_GUARD ;
			pl_w->cpu_cancel_flag |= ~0x7f ;
		}
	}
	return 1 ;
}

/****************************************************************/
int cmd_flag_ck( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( pl_w->cpu_cancel_flag ){
		if( pl_w->cpu_cancel_flag > 0 ){
			pl_w->cpu_cancel_flag = 0 ;
			goto_endif( pl_w, sc ) ;
			return 0 ;
		} else if( !( pl_w->cpu_cancel_flag & CC_GUARD ) ){
			pl_w->cpu_cancel_flag &= CC_ON | CC_SKIP ;
		} else if( pl_w->cpu_cancel_flag & CC_SKIP ){
			pl_w->cpu_cancel_flag = 0 ;
			goto_endif( pl_w, sc ) ;
			return 0 ;
		} else {
			pl_w->cpu_cancel_flag &= ~CC_GO ;
		}
	}
	return 1 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "em_sub.c" End of File	*/
