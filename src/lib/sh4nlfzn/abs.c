/* abs: the absolute value of an int, from Hitachi's run-time library. */

int abs( int n )
{
	if( n > 0 )
		return n;
	return -n;
}
