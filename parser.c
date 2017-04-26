#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

void parse_file(char * filename, struct stack * coordStack, struct matrix * edges, struct matrix * polygons, screen s)
{
  FILE *f;
  char line[255];
  clear_screen(s);

  color c;
  c.red = 0;
  c.green = 0;
  c.blue = 0;

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");

  while( fgets(line, sizeof(line), f) != NULL) {
    line[strlen(line) - 1] = '\0';
    //printf(":%s:\n",line);

    double xvals[4];
    double yvals[4];
    double zvals[4];
    struct matrix *tmp;
    double r, r1;
    double theta;
    char axis;
    int type;
    double step = 0.06;

    if(strncmp(line, "push", strlen(line)) == 0) {
      push(coordStack);
    }
    else if(strncmp(line, "pop", strlen(line)) == 0) {
      pop(coordStack);
    }
    else if(strncmp(line, "box", strlen(line)) == 0) {
      fgets(line, sizeof(line), f);
      //printf("BOX\t%s", line);

      sscanf(line, "%lf %lf %lf %lf %lf %lf", xvals, yvals, zvals, xvals + 1, yvals + 1, zvals + 1);
      add_box(polygons, xvals[0], yvals[0], zvals[0], xvals[1], yvals[1], zvals[1]);

      //transform the box
      struct matrix * topTrans = coordStack->data[coordStack->top];
      matrix_mult(topTrans, polygons);

      //draw the box
      draw_polygons(polygons, s, c);

      //clear polygons
      polygons->lastcol = 0;
    }
    else if(strncmp(line, "sphere", strlen(line)) == 0) {
      fgets(line, sizeof(line), f);
      //printf("SPHERE\t%s", line);

      sscanf(line, "%lf %lf %lf %lf", xvals, yvals, zvals, &r);
      add_sphere(polygons, xvals[0], yvals[0], zvals[0], r, step);

      //transform the sphere
      struct matrix * topTrans = coordStack->data[coordStack->top];
      matrix_mult(topTrans, polygons);

      //draw the sphere
      draw_polygons(polygons, s, c);

      //clear polygons
      polygons->lastcol = 0;
    }
    else if(strncmp(line, "torus", strlen(line)) == 0) {
      fgets(line, sizeof(line), f);
      //printf("torus\t%s", line);

      sscanf(line, "%lf %lf %lf %lf %lf", xvals, yvals, zvals, &r, &r1);
      add_torus(polygons, xvals[0], yvals[0], zvals[0], r, r1, step);

      //transform the torus
      struct matrix * topTrans = coordStack->data[coordStack->top];
      matrix_mult(topTrans, polygons);

      //draw the torus
      draw_polygons(polygons, s, c);

      //clear polygons
      polygons->lastcol = 0;
    }
    else if(strncmp(line, "circle", strlen(line)) == 0) {
      fgets(line, sizeof(line), f);
      //printf("CIRCLE\t%s", line);

      sscanf(line, "%lf %lf %lf %lf", xvals, yvals, zvals, &r);
      add_circle(edges, xvals[0], yvals[0], zvals[0], r, step);

      //transform the circle
      struct matrix * topTrans = coordStack->data[coordStack->top];
      matrix_mult(topTrans, edges);

      //draw the circle
      draw_lines(edges, s, c);

      //clear edges
      edges->lastcol = 0;
    }
    else if(strncmp(line, "hermite", strlen(line)) == 0 || strncmp(line, "bezier", strlen(line)) == 0) {
      if(strncmp(line, "hermite", strlen(line)) == 0)
        type = HERMITE;
      else
        type = BEZIER;

      fgets(line, sizeof(line), f);
      //printf("CURVE\t%s", line);

      sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", xvals, yvals, xvals + 1, yvals + 1, xvals + 2, yvals + 2, xvals + 3, yvals + 3);
      /* printf("%lf %lf %lf %lf %lf %lf %lf %lf\n", */
      /*       xvals[0], yvals[0], */
      /*       xvals[1], yvals[1], */
      /*       xvals[2], yvals[2], */
      /*       xvals[3], yvals[3]); */

      //printf("%d\n", type);
      add_curve(edges, xvals[0], yvals[0], xvals[1], yvals[1], xvals[2], yvals[2], xvals[3], yvals[3], step, type);

      //transform the curve
      struct matrix * topTrans = coordStack->data[coordStack->top];
      matrix_mult(topTrans, edges);

      //draw the curve
      draw_lines(edges, s, c);

      //clear edges
      edges->lastcol = 0;
    }
    else if(strncmp(line, "line", strlen(line)) == 0) {
      fgets(line, sizeof(line), f);
      //printf("LINE\t%s", line);

      sscanf(line, "%lf %lf %lf %lf %lf %lf", xvals, yvals, zvals, xvals + 1, yvals + 1, zvals + 1);
      /*printf("%lf %lf %lf %lf %lf %lf",
      xvals[0], yvals[0], zvals[0],
      xvals[1], yvals[1], zvals[1]) */
      add_edge(edges, xvals[0], yvals[0], zvals[0], xvals[1], yvals[1], zvals[1]);

      //transform the line
      struct matrix * topTrans = coordStack->data[coordStack->top];
      matrix_mult(topTrans, edges);

      //draw the line
      draw_lines(edges, s, c);

      //clear edges
      edges->lastcol = 0;
    }
    else if(strncmp(line, "scale", strlen(line)) == 0) {
      fgets(line, sizeof(line), f);
      //printf("SCALE\t%s", line);
      sscanf(line, "%lf %lf %lf", xvals, yvals, zvals);
      /* printf("%lf %lf %lf\n", */
      /*  xvals[0], yvals[0], zvals[0]); */ 
      tmp = make_scale(xvals[0], yvals[0], zvals[0]);

      //modify coordStack
      struct matrix * topTrans = coordStack->data[coordStack->top];
      matrix_mult(tmp, topTrans);

      free_matrix(tmp);
    }
    else if(strncmp(line, "move", strlen(line)) == 0) {
      fgets(line, sizeof(line), f);
      //printf("MOVE\t%s", line);
      sscanf(line, "%lf %lf %lf", xvals, yvals, zvals);
      /* printf("%lf %lf %lf\n", */
      /*  xvals[0], yvals[0], zvals[0]); */ 
      tmp = make_translate(xvals[0], yvals[0], zvals[0]);

      //modify coordStack
      struct matrix * topTrans = coordStack->data[coordStack->top];
      matrix_mult(tmp, topTrans);

      free_matrix(tmp);
    }
    else if(strncmp(line, "rotate", strlen(line)) == 0) {
      fgets(line, sizeof(line), f);
      //printf("Rotate\t%s", line);
      sscanf(line, "%c %lf", &axis, &theta);
      /* printf("%c %lf\n", */
      /*  axis, theta); */
      theta = theta * (M_PI / 180);
      if(axis == 'x')
        tmp = make_rotX(theta);
      else if(axis == 'y')
        tmp = make_rotY(theta);
      else
        tmp = make_rotZ(theta);

      //modify coordStack
      struct matrix * topTrans = coordStack->data[coordStack->top];
      matrix_mult(tmp, topTrans);

      free_matrix(tmp);
    }
    else if(strncmp(line, "clear", strlen(line)) == 0) {
      //printf("clear\t%s", line);
      edges->lastcol = 0;
      polygons->lastcol = 0;
    }
    else if(strncmp(line, "ident", strlen(line)) == 0) {
      //printf("IDENT\t%s", line);
      ident(transform);
    }
    else if(strncmp(line, "apply", strlen(line)) == 0) {
      //printf("APPLY\t%s", line);
      if(edges->lastcol >= 2) matrix_mult(transform, edges);
      if(polygons->lastcol >= 3) matrix_mult(transform, polygons);
    }
    else if(strncmp(line, "display", strlen(line)) == 0) {
      //printf("DISPLAY\t%s", line);
      clear_screen(s);
      draw_polygons(polygons, s, c);
      draw_polygons(edges, s, c);
      display(s);
    }
    else if(strncmp(line, "save", strlen(line)) == 0) {
      fgets(line, sizeof(line), f);
      *strchr(line, '\n') = 0;
      //printf("SAVE\t%s\n", line);
      clear_screen(s);
      draw_polygons(polygons, s, c);
      draw_lines(edges, s, c);
      save_extension(s, line);
    }
  }
}