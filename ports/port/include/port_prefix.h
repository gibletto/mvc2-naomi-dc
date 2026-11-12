/* port_prefix.h: included before every game file in a host build. It stands in for what SHC
   supplied: its built-in routines, and the single-precision float of -fpu=single. */
#ifndef PORT_PREFIX_H
#define PORT_PREFIX_H

#define HOST_PORT 1
#define _STRICT_UNION_ 1
#define __DEV_TYPE_DC__ 1

int host_get_imask( void ) ;
void host_set_imask( int mask ) ;
#define _builtin_get_imask()		host_get_imask()
#define _builtin_set_imask( m )		host_set_imask( m )
#define _builtin_get_fpscr()		0x00040001
#define _builtin_set_fpscr( v )		( ( void )( v ) )
#define _builtin_sqrtf( x )		__builtin_sqrtf( x )
#define _builtin_fabsf( x )		__builtin_fabsf( x )

#endif
