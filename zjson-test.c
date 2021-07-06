#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "zjson.h"

#define TESTDIR "tests/"

struct test { const char *file; } tests[] = {
	{ TESTDIR "lil.json" }
#if 0
,	{ TESTDIR "not.json" }
,	{ TESTDIR "singles.json" }
,	{ TESTDIR "twitter.json" }
#endif
,	{ NULL }
};


int main (int argc, char *argv[]) {
	for ( struct test *t = tests; t->file; t++ ) {
		int fd = 0;
		struct stat sb;
		char err[1024] = {0}, *base = NULL;
		zTable *tt = NULL;

		//Open the file and string
		if ( ( fd = open( t->file, O_RDONLY ) ) == -1 || stat( t->file, &sb ) == -1 ) {
			fprintf( stderr, "Open failed: %s\n", strerror( errno ) );
			continue;
		}

		if ( !( base = malloc( sb.st_size ) ) || !memset( base, 0, sb.st_size ) ) {
			fprintf( stderr, "malloc failed: %s\n", strerror( errno ) );
			continue;
		}

		if ( read( fd, base, sb.st_size ) == -1 ) {
			fprintf( stderr, "read failed: %s\n", strerror( errno ) );
			continue;
		}

		write( 2, base, sb.st_size );

		//Encode (and decode?)
		if ( !( tt = zjson_encode( base, sb.st_size, err, sizeof( err ) ) ) ) {
			fprintf( stderr, "encode failed: %s\n", err );
			continue;
		}
		
		//Make sure you catch failures too
		//Close the file
		close( fd );
		free( base );
	}
	return 0;
}
