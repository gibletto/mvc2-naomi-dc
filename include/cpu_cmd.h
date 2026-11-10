/* The computer's script commands, as a character's command lists write them (data/taikiXX.c,
   atckXX.c, wariXX.c, yokXX.c and libXX.c). A list is bytes: a command's number, then its
   parameters. em_script.c has the routine for each command, and tools/chardata.py takes every
   command's form from this file.

   A command from IF_DIST on (96 and above) opens a block that CMD_ENDIF closes, with a CMD_ELSE
   between for a test: where the test fails, or the thing the block waits on is over, the script
   skips to the block's end. A distance is in the dots of Capcom's older boards; the program
   scales it. */
#ifndef CPU_CMD_H
#define CPU_CMD_H

/* a word, its high byte first */
#define CMD_W( n )	( ( n ) >> 8 & 0xff ), ( ( n ) & 0xff )
/* a number with no routine: nothing is done */
#define CMD_NONE( no )	( no )

/* The parameters. */

/* dir: the way of a jump (the work's jump_dir) */
#define DIR_FORWARD	0x00
#define DIR_BACK	0x01
#define DIR_UP		0xff	/* straight up */

/* level and pk: the button of an attack (the work's atck_level and atck_pk) */
#define LV_LIGHT	0x00
#define LV_HEAVY	0x01
#define PK_PUNCH	0x00
#define PK_KICK		0x01

/* lever: the stick, as the high byte of key.h's words; forward and back are turned by the facing */
#define LEVER_BACK	0x04
#define LEVER_FORWARD	0x08
#define LEVER_DOWN	0x10
#define LEVER_UP	0x20

/* renda: the way Omega Red holds the stick as he mashes the heavy punch, every other frame, while
   his move's routine_2 is 5 (no other character reads it) */
#define RENDA_BACK	0x01
#define RENDA_DOWN	0x02
#define RENDA_UP	0x03
#define RENDA_UP_BACK	0x04

/* The commands. */

#define CMD_NOP			0
#define WALK_F( x )		1, CMD_W( x )	/* walks forward a distance */
#define WALK_B( x )		2, CMD_W( x )	/* walks back a distance, unless an attack is coming */
#define CMD_RESET		3	/* picks a waiting list (taikiXX) again */
#define WAIT( time )		4, ( time )	/* waits a number of frames */
/* waits a time from the program's tables: the number picks a pair of them, a wait by the level of
   play and a step taken at random */
#define WAIT2( time )		5, ( time )
#define CROUCH			6
#define STAND			7
#define CMD_EXIT		8	/* leaves the list: the waiting list goes on from where it was */
#define CMD_NEXT		9	/* goes over to an attack list (atckXX) if the waiting lists' rate says so; if not, as CMD_EXIT */
#define CMD_NEXT3		10	/* goes over to an attack list */
#define PUNCH( level )		11, ( level )	/* presses a punch, LV_LIGHT or LV_HEAVY, or lets go of one held */
#define KICK( level )		12, ( level )	/* the same for a kick */
#define JUMP( dir )		13, ( dir )	/* jumps: DIR_FORWARD, DIR_UP or DIR_BACK */
#define WALK_EM( x )		14, CMD_W( x )	/* walks up to a distance from the opponent */
#define CMD_JUMP( no )		15, ( no )	/* goes to a list of libXX, by its number */
#define SJUMP( dir )		16, ( dir )	/* super jumps, as JUMP */
/* does a special move: its number (plXX_sp_ in plXX_def.h), Omega Red's mashing after it, its
   button (pk and level) and the stick */
#define SP_CMD( no, renda, pk, level, lever )	17, ( no ), ( renda ), ( pk ), ( level ), ( lever )
#define DASH_F( x )		18, CMD_W( x )	/* dashes forward a distance */
#define DASH_B( x )		19, CMD_W( x )	/* dashes back a distance, unless an attack is coming */
#define DASH_EM( x )		20, CMD_W( x )	/* dashes up to a distance from the opponent */
#define AIR_JUMP( dir )		21, ( dir )	/* lets it make an air jump (cmd_size_tbl counts a byte more) */
#define JUMP_RET		22	/* goes back to the list it came from */
#define CMD_ELSE		23
#define CMD_ENDIF		24
#define TIMER_SET( time )	25, ( time )	/* sets the frames an if keeps trying; 0 for 120, 90, 60 or 30 at random */
#define GUARD( time )		26, ( time )	/* guards for a number of frames, if an attack is coming */
#define SJUMP2( dir )		27, ( dir )	/* super jumps, when a command may start */
#define AIR_WAIT( time, n )	28, ( time ), ( n )	/* waits a number of frames, in the air; n is not read */
#define CMD_DUMMY		29	/* does nothing */
#define JUMP_RND( a, b, c, d )	30, ( a ), ( b ), ( c ), ( d )	/* goes to one of four lists of libXX, at random */
#define CANCEL_CLR		31	/* clears the wait CANCEL_SET set */
/* goes to one of eight lists of libXX, by the opponent's weight */
#define JUMP_EM( a, b, c, d, e, f, g, h )	32, ( a ), ( b ), ( c ), ( d ), ( e ), ( f ), ( g ), ( h )
/* goes to a list of libXX by the level of play: the first whose level is above it, or the last */
#define JUMP_LEVEL( no0, lv0, no1, lv1, no2, lv2, no3 )	33, ( no0 ), ( lv0 ), ( no1 ), ( lv1 ), ( no2 ), ( lv2 ), ( no3 )
#define JUMP_FLAG_CLR		34	/* ends JUMP_FLAG_SET's or SJUMP_FLAG_SET's watch */
#define LOCK_SET		35	/* keeps the computer in its mode and list */
#define LOCK_CLR		36
#define GUARD_CONT( time )	37, ( time )	/* guards on, for a number of frames, while on point */
#define WALK_RND( no )		38, ( no )	/* walks to a distance from the opponent picked at random from a table */
/* goes to the first list of libXX whose distance is beyond the opponent; the last byte is not read
   (cmd_size_tbl counts six bytes more) */
#define JUMP_DIST( no0, x0, no1, x1, no2, x2, n )	39, ( no0 ), CMD_W( x0 ), ( no1 ), CMD_W( x1 ), ( no2 ), CMD_W( x2 ), ( n )
#define TIMER2_SET( time )	40, ( time )	/* guards without its chance check for a time, or one picked at random */
#define VCOUNTER_SET( n )	42, ( n )	/* after n more hits on it, its guard turns to a variable counter */
#define NAG_VFLIP_ADJ( a, lever )	43, ( a ), ( lever )	/* a throw: the stick with the heavy punch; a is not read */
#define NAG_VFLIP_ADJ2( a, lever )	44, ( a ), ( lever )	/* the same with the heavy kick */
#define CATCH_NG_SET( time )	47, ( time )	/* can't be thrown, or hit, for a number of frames */
#define WALK_DIST( x )		49, CMD_W( x )	/* walks to a distance from the opponent */
#define WALK_F2( x )		50, CMD_W( x )	/* 50 to 55 read their distance and do nothing */
#define WALK_B2( x )		51, CMD_W( x )
#define WALK_F3( x )		52, CMD_W( x )
#define WALK_B3( x )		53, CMD_W( x )
#define WALK_F4( x )		54, CMD_W( x )
#define WALK_B4( x )		55, CMD_W( x )
#define NAG_VFLIP_ADJ3( level, lever )	57, ( level ), ( lever )	/* a throw: the stick with a punch, LV_LIGHT or LV_HEAVY */
#define NAG_VFLIP_ADJ4( level, lever )	58, ( level ), ( lever )	/* and with a kick */
#define GUARD2( time )		60, ( time )	/* as GUARD, crouching */
#define FLY_FLAG_CLR		62	/* ends FLY_FLAG_SET's watch, and the flight */
#define WALK_EM2( x, up )	63, CMD_W( x ), ( up )	/* as WALK_EM for a flier, holding the stick up too unless up is 0 */
#define WALK_POS( x, up )	64, CMD_W( x ), ( up )	/* goes to a place beside the opponent, from either side, in the same way */
#define FLY( lever, x )		65, ( lever ), CMD_W( x )	/* flies a distance, the stick given */
#define WALK_POS_Y( x, y )	66, CMD_W( x ), CMD_W( y )	/* goes to a place beside the opponent and at a height, for a flier */
#define CMD_NEXT2		67	/* as CMD_NEXT, by the attack lists' rate */
#define SP_CHANGE		68	/* tags a partner in */
#define SP_V_COMBI		69	/* starts the team's hyper combo */
#define SJUMP3( dir )		70, ( dir )	/* super jumps, as a cancel */
#define GUARD_BACK_SET( n )	71, ( n )	/* after n more hits on it, its guard pushes the attacker back */
#define PARTNER_CALL		72	/* calls a partner to assist */
#define CMD_HIENDIF		73	/* the endif that the end of a flight skips to */
#define PUNCH_HOLD( level )	74, ( level )	/* holds a punch, LV_LIGHT or LV_HEAVY */
#define KICK_HOLD( level )	75, ( level )	/* holds a kick */
#define CMD_STOP		76	/* waits */
#define PARTNER_CHANGE		77	/* tags in the partner with the least vitality */
/* a special move to do later, if the gauge is at a level: SP_CMD's numbers, the level, and the
   frames to wait (cmd_size_tbl counts the command's number alone) */
#define SP_CMD_SET( no, renda, pk, level, lever, gauge, time )	78, ( no ), ( renda ), ( pk ), ( level ), ( lever ), ( gauge ), CMD_W( time )

/* if the opponent is within a box around a point: x and its half width, y and its half height */
#define IF_DIST( x, w, y, h )	96, CMD_W( x ), CMD_W( w ), CMD_W( y ), CMD_W( h )
/* if its own attack no (a row of hit_dtXX) would reach the opponent: one of the row's two boxes
   meets a body box */
#define IF_EM_ATCK( no )	97, ( no )
#define CANCEL_SET		98	/* the next command waits for the one before */
/* if a box beside it reaches the opponent's body: its half width and half height, then its
   centre's place across and up */
#define IF_AREA( w, h, x, y )	99, CMD_W( w ), CMD_W( h ), CMD_W( x ), CMD_W( y )
#define CANCEL_SET2		100	/* as CANCEL_SET, and skips to the endif if it can't */
#define JUMP_FLAG_SET		101	/* once the jump is over, the script skips to the endif */
#define SJUMP_FLAG_SET		102	/* the same for a super jump */
#define WALK_ATCK( no )		103, ( no )	/* walks forward until its attack no would reach the opponent */
#define DASH_ATCK( no )		104, ( no )	/* dashes forward until its attack no would reach the opponent */
#define IF_CATCH		105	/* if a throw would catch the opponent, with a punch or a kick */
#define IF_LEVEL( level )	106, ( level )	/* if the level of play is at a number or above */
/* as IF_AREA, for the attack boxes of the other side's attacks under way */
#define IF_AREA2( w, h, x, y )	107, CMD_W( w ), CMD_W( h ), CMD_W( x ), CMD_W( y )
#define FLY_FLAG_SET		108	/* once the flight is over, the script skips to its CMD_HIENDIF */
#define IF_RND( rate )		109, ( rate )	/* at random: the chance, of 32 */
#define IF_ATCK_COME		110	/* if an attack is coming, and its own attack of that number would reach the attacker */
#define IF_GAUGE( level )	111, ( level )	/* if the hyper combo gauge is at a level or above */
#define IF_FIGHT_SEC( sec )	112, ( sec )	/* if it has fought on point for more than a number of seconds */
/* if it is at a height or above and not falling; with dir 0xff, if it is below the height and not
   rising */
#define IF_HEIGHT( dir, y )	113, ( dir ), CMD_W( y )
#define IF_TLOCAL		114	/* if the character's own work has its flag 14 clear */
#define IF_TIME( time )		115, ( time )	/* if the count from 99 has gone past a number */
#define IF_EM_STANCE( stance )	116, ( stance )	/* if the opponent's stance is this one: STANCE_STAND, STANCE_CROUCH or STANCE_AIR */
/* if the character's own count is at a number (Silver Samurai, Spiral, Sakura and Shuma-Gorath
   keep one) */
#define IF_CHAR( n )		117, ( n )
#define IF_TLOCAL2( n )		118, ( n )	/* if the character's own work starts with a number */
#define IF_VITAL( n )		119, CMD_W( n )	/* if its vitality is at or below a share of the most, out of 200 */
#define IF_EM_VITAL( n )	120, CMD_W( n )	/* the same for the opponent */
#define IF_NG( n )		121, CMD_W( n )	/* a test that never holds */
#define IF_NG2( n )		122, CMD_W( n )	/* and another */
#define IF_TEAM_VITAL( n )	123, CMD_W( n )	/* if its team's vitality is at or below a share */
#define IF_EM_TEAM_VITAL( n )	124, CMD_W( n )	/* the same for the opponent's team */
#define IF_EM_JUMP( dir )	126, ( dir )	/* if the opponent's jump went this way */

#endif
