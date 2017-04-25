#ifndef PARSER_H
#define PARSER_H

#include "matrix.h"
#include "ml6.h"

void parse_file ( char * filename, 
		  struct matrix * transform, 
		  struct matrix * edges,
		  struct matrix * polygons,
		  screen s);

#endif