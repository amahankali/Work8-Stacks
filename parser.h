#ifndef PARSER_H
#define PARSER_H

#include "matrix.h"
#include "stack.h"
#include "ml6.h"

void parse_file ( char * filename, 
		  struct stack * coordStack, 
		  struct matrix * edges,
		  struct matrix * polygons,
		  screen s);

#endif