#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main(int argc, char **argv) {

  screen s;
  struct matrix * edges;
  struct matrix * polygons;
  struct stack * coordStack;

  edges = new_matrix(4, 4);
  polygons = new_matrix(4, 4);
  coordStack = new_stack();

  //print_matrix(make_bezier());
  //printf("\n");
  //print_matrix(make_hermite());

  if(argc == 2)
    parse_file(argv[1], coordStack, edges, polygons, s);
  else
    parse_file("stdin", coordStack, edges, polygons, s);

  free_stack(coordStack);
  free_matrix(edges);
  free_matrix(polygons);
}  