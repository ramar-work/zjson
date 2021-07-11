#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "zjson.h"

#define TESTDIR "tests/"

struct test { const char *file; } tests[] = {
	{ TESTDIR "lil.json" },
#if 0
 	{ TESTDIR "not.json" },
 	{ TESTDIR "singles.json" },
 	{ TESTDIR "twitter.json" },
 	{ TESTDIR "package.json" },
#endif
 	{ NULL }
};


int main (int argc, char *argv[]) {

	for ( struct test *t = tests; t->file; t++ ) {
		int fd = 0;
		struct stat sb;
		char err[1024] = {0}, *base = NULL, *enc = NULL;
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

		//write( 2, base, sb.st_size );
		if ( zjson_check( base, sb.st_size, err, sizeof ( err ) ) ) 
			fprintf( stderr, "JSON check passed!\n" );
		else {
			fprintf( stderr, "JSON check failed: %s\n", err ); 
			continue;
		}	

		//Encode (and decode?)
		if ( !( tt = zjson_decode( base, sb.st_size, err, sizeof( err ) ) ) ) {
			fprintf( stderr, "decode failed: %s\n", err );
			continue;
		}

	#if 0
		//Dump	
		lt_kfdump( tt, 2 );
	#else
		//Make sure you catch failures too
		if ( !( enc = zjson_encode( tt, err, sizeof( err ) ) ) ) {
			fprintf( stderr, "encode failed: %s\n", err );
			continue;
		}
	
		//...
		fprintf( stderr, "JSON: %s\n", enc );	
	#endif

		//Destroy everything
		free( enc );
		lt_free( tt ), free( tt ); 
		free( base ), close( fd );
	}
	return 0;
}
