#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ztable.h>
#include <zwalker.h>


#define JSON_MAX_DEPTH 100


zTable * zjson_encode ( const char *, int, char *, int );
char * zjson_decode ( const zTable * ); 
