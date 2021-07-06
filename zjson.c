/* ------------------------------------------- * 
 * json.c 
 * =========
 * 
 * Summary 
 * -------
 * JSON deserialization / serialization 
 *
 * LICENSE
 * -------
 * Copyright 2020 Tubular Modular Inc. dba Collins Design
 * 
 * See LICENSE in the top-level directory for more information.
 *
 * CHANGELOG 
 * ---------
 * No entries yet.
 *
 * ------------------------------------------- */
#include "zjson.h"

typedef struct { int text, type, isval, index; unsigned char *val; int size; } Depth; 

//Trim any characters 
static unsigned char *zjson_trim 
	( unsigned char *msg, char *trim, int len, int *nlen ) {
	//Define stuff
	unsigned char *m = msg;
	int nl= len;
	//Move forwards and backwards to find whitespace...
	while ( memchr(trim, *(m + ( nl - 1 )), strlen(trim)) && nl-- ) ; 
	while ( memchr(trim, *m, strlen(trim)) && nl-- ) m++;
	*nlen = nl;
	return m;
}



//Get an approximation of the number of keys needed
static int zjson_count ( unsigned char *src, int len ) {
	int sz = 0;

	//You can somewhat gauge the size needed by looking for all commas
	for ( int c = len; c; c-- ) {
		sz += ( memchr( "{[,]}", *src,  5 ) ) ? 1 : 0, src++;
	}
	return sz;
}



//This is the important one
zTable * zjson_encode ( const char *str, int len, char *err, int errlen ) {
	const char tokens[] = "\"{[}]:,\\"; // this should catch the backslash
	unsigned char *b = NULL;
	//int size = zjson_count( (unsigned char *)str, strlen( str ) );
	zWalker w = {0};
	zTable *tt = NULL;
	Depth depths[ JSON_MAX_DEPTH ], *d = depths;
	memset( depths, 0, JSON_MAX_DEPTH * sizeof( Depth ) );
	struct bot { char *key; unsigned char *val; int size; } bot;

	//Return zTable
	if ( !( tt = lt_make( 10000 * 2 ) ) ) {
		snprintf( err, errlen, "%s", "Create table failed." );
		return NULL;
	}

	//Add a root key
	lt_addtextkey( tt, "root" );
	//lt_descend( tt );

	for ( int i = 0; memwalk( &w, (unsigned char *)str, (unsigned char *)tokens, len, strlen( tokens ) ); ) {
		if ( w.chr == '"' ) {
			//b = json_trim( &w.ptr[ p.prev ], "\"' \t\n\r", p.size, &adjust );
			if ( ( d->text = !d->text ) == 1 )
				d->val = w.ptr, d->size = 0;
			else {
				d->size += w.size - 1;
				char buf[ d->size + 1 ];
				memset( buf, 0, d->size + 1 );
				//Left or right side...
				if ( !d->isval ) {
					memcpy( buf, d->val, d->size );
					lt_addtextkey( tt, buf );
				}
				else {
					memcpy( buf, d->val, d->size - 2 );
					lt_addtextvalue( tt, buf );
					lt_finalize( tt );
					d->isval = 0;
					d->size = 0;
				}
			}
		}

		//Need to check if we are within a string...
		if ( d->text && d->isval )
			d->size += w.size;
		else {
			if ( w.chr == ':' )
				d->isval = 1;	
			else if ( w.chr == '{' || w.chr == '[' ) {
				++d, ++i, lt_descend( tt );
				if ( w.chr == '[' ) {
					d->type = 1, lt_addintkey( tt, d->index ), d->index++;
				}
			}
			else if ( w.chr == ',' ) {
				if ( d->type ) {
					lt_addintkey( tt, d->index ), d->index++;
				}
	
				//Trim the value
				int a = 0;
				unsigned char *aa = zjson_trim( w.src, " \t\n\r", w.size, &a );
#if 0
fprintf( stderr, "Got a value that might not be a string. %d\n", a ); 
write( 2, aa, a );
getchar();
#endif

				if ( a > 1 ) {
					char buf[ a ];
					memset( buf, 0, a );
					memcpy( buf, aa, a - 1 );
					lt_addtextvalue( tt, buf );
					lt_finalize( tt );
				}
				d->isval = 0, d->text  = 0;
			}
			else if ( w.chr == '}' || w.chr == ']' ) {
				//if ( d != &depths[ 0 ] ) { 
				if ( --i == -1 ) {
					break;	
				}
				--d, d->isval = 0, d->size = 0, d->text = 0, lt_ascend( tt );
			}
		}
#if 0
//fprintf( stderr, "aindex %d, ", i );fprintf( stderr, "%p ?= %p\n", d, &depths[ 0 ] );
fprintf( stderr, "tp: %p, tindex: %d, ", d, i );
fprintf( stderr, "side: %s, ", !d->isval ? "L" : "R" );
fprintf( stderr, "narray: %s, ", d->type ? "true" : "false" );
fprintf( stderr, "size: %d, chr: %c, ptr: ", w.size, w.chr );
write( 2, w.src, w.size );
getchar();
#endif
	}

lt_lock( tt );
	return tt;
}



//...
char * zjson_decode ( const zTable *str ) {
	zWalker set = {0};
	return NULL;
}




