/********************************************************* -- SOURCE -{{{1- */
/** Translate host name into IPv4
 *
 * Resolve IPv4 address for a given host name. The host name is specified as
 * the first command line argument to the program. 
 *
 * Build program:
 *  $ gcc -Wall -g -o resolve <file>.cpp
 */
/******************************************************************* -}}}1- */

#include <stdio.h>
#include <stddef.h>

#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>

//--//////////////////////////////////////////////////////////////////////////
//--    local declarations          ///{{{1///////////////////////////////////

void print_usage( const char* aProgramName );

//--    local config                ///{{{1///////////////////////////////////

/* HOST_NAME_MAX may be missing, e.g. if you're running this on an MacOS X
 * machine. In that case, use MAXHOSTNAMELEN from <sys/param.h>. Otherwise 
 * generate an compiler error.
 */
#if !defined(HOST_NAME_MAX)
#	if defined(__APPLE__)
#		include <sys/param.h>
#		define HOST_NAME_MAX MAXHOSTNAMELEN
#	else  // !__APPLE__
#		error "HOST_NAME_MAX undefined!"
#	endif // ~ __APPLE__
#endif // ~ HOST_NAME_MAX

//--    main()                      ///{{{1///////////////////////////////////
int main( int aArgc, char* aArgv[] )
{
	// Check if the user supplied a command line argument.
	if( aArgc != 2 )
	{
		print_usage( aArgv[0] );
		return 1;
	}

	// The (only) argument is the remote host that we should resolve.
	const char* remoteHostName = aArgv[1];

	// Get the local host's name (i.e. the machine that the program is 
	// currently running on).
	const size_t kHostNameMaxLength = HOST_NAME_MAX+1;
	char localHostName[kHostNameMaxLength];

	if( -1 == gethostname( localHostName, kHostNameMaxLength ) )
	{
		perror( "gethostname(): " );
		return 1;
	}

	// Print the initial message
	printf( "Resolving `%s' from `%s':\n", remoteHostName, localHostName );

	// TODO : add your code here
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s;
	char str[INET_ADDRSTRLEN];

	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_canonname = 0;
	hints.ai_addr = 0;
	hints.ai_next = 0;

	s = getaddrinfo(remoteHostName, NULL, &hints, &result);
	if( s != 0 ){
	  fprintf(stderr, "Error in getaddrinfo: %s\n", gai_strerror(s));
	  exit(EXIT_FAILURE);
	}

	for ( rp = result; rp != NULL; rp = rp->ai_next) {
	  sockaddr* sa = rp->ai_addr;
	  sockaddr_in* ia = (sockaddr_in*)sa;
	  //uint32_t ip = ia->sin_addr.s_addr;
	  int port = ia->sin_port;
	  fprintf(stdout, "%s has address %s\n", remoteHostName, inet_ntop(AF_INET, &(ia->sin_addr), str, INET_ADDRSTRLEN));
	}

	freeaddrinfo(result);

	// Ok, we're done. Return success.
	return 0;
}


//--    print_usage()               ///{{{1///////////////////////////////////
void print_usage( const char* aProgramName )
{
	fprintf( stderr, "Usage: %s <hostname>\n", aProgramName );
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
