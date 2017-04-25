#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "math.h"

/*======== void add_polygon() ==========
Inputs:   struct matrix *surfaces
         double x0
         double y0
         double z0
         double x1
         double y1
         double z1
         double x2
         double y2
         double z2  
Returns: 
Adds the vertices (x0, y0, z0), (x1, y1, z1)
and (x2, y2, z2) to the polygon matrix. They
define a single triangle surface.
====================*/
void add_polygon( struct matrix *polygons, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2 ) {
  add_point(polygons, x0, y0, z0);
  add_point(polygons, x1, y1, z1);
  add_point(polygons, x2, y2, z2);
}

/*======== void draw_polygons() ==========
Inputs:   struct matrix *polygons
          screen s
          color c  
Returns: 
Goes through polygons 3 points at a time, drawing 
lines connecting each points to create bounding
triangles
====================*/
void draw_polygons( struct matrix *polygons, screen s, color c ) {

  int i = 0;
  for(; i < polygons->lastcol; i += 3)
  {
    double x0, y0, z0;
    x0 = polygons->m[0][i];
    y0 = polygons->m[1][i];
    z0 = polygons->m[2][i];

    double x1, y1, z1;
    x1 = polygons->m[0][i + 1];
    y1 = polygons->m[1][i + 1];
    z1 = polygons->m[2][i + 1];

    double x2, y2, z2;
    x2 = polygons->m[0][i + 2];
    y2 = polygons->m[1][i + 2];
    z2 = polygons->m[2][i + 2];

    //determine if polygon should be drawn
    double ax = (x1 - x0);
    double ay = (y1 - y0);

    double bx = (x2 - x0);
    double by = (y2 - y0);

    //z component of cross product - dot product of normal vector and view vector <0, 0, 1>
    double cz = ax * by - ay * bx;

    if(cz >= 0)
    {
      draw_line(x0, y0, x1, y1, s, c);
      draw_line(x0, y0, x2, y2, s, c);
      draw_line(x1, y1, x2, y2, s, c);
    }
  }


}


/*======== void add_box() ==========
  Inputs:   struct matrix * edges
            double x
	    double y
	    double z
	    double width
	    double height
	    double depth
  Returns: 

  add the points for a rectagular prism whose 
  upper-left corner is (x, y, z) with width, 
  height and depth dimensions.
  ====================*/
void add_box( struct matrix * edges, double x, double y, double z, double width, double height, double depth ) {

  double xA, yA, zA, xB, yB, zB;
  xA = x;
  xB = x + width;
  yA = y;
  yB = y - height;
  zA = z;
  zB = z - depth;

  struct matrix* points = new_matrix(4, 8);
  points->lastcol = 0;
  add_point(points, xA, yA, zA);
  add_point(points, xA, yA, zB);
  add_point(points, xB, yA, zB);
  add_point(points, xB, yA, zA);
  add_point(points, xA, yB, zA);
  add_point(points, xA, yB, zB);
  add_point(points, xB, yB, zB);
  add_point(points, xB, yB, zA);

  //top face
  int c = 0;
  int num_steps = 4;
  for(; c < num_steps; c += 2)
  {
    int i1 = c;
    int i2 = (c + num_steps - 1) % num_steps;
    int i3 = (c + 1) % num_steps;

    //p1
    double x1 = points->m[0][i1];
    double y1 = points->m[1][i1];
    double z1 = points->m[2][i1];

    //p2
    double x2 = points->m[0][i2];
    double y2 = points->m[1][i2];
    double z2 = points->m[2][i2];

    //p3
    double x3 = points->m[0][i3];
    double y3 = points->m[1][i3];
    double z3 = points->m[2][i3];

    add_polygon(edges, x1, y1, z1, x2, y2, z2, x3, y3, z3);
  }

  //bottom face
  for(; c < 2 * num_steps; c += 2)
  {
    int i1 = c;
    int i2 = (c + 1) % num_steps + num_steps;
    int i3 = (c + num_steps - 1) % num_steps + num_steps;

    //p1
    double x1 = points->m[0][i1];
    double y1 = points->m[1][i1];
    double z1 = points->m[2][i1];

    //p2
    double x2 = points->m[0][i2];
    double y2 = points->m[1][i2];
    double z2 = points->m[2][i2];

    //p3
    double x3 = points->m[0][i3];
    double y3 = points->m[1][i3];
    double z3 = points->m[2][i3];

    add_polygon(edges, x1, y1, z1, x2, y2, z2, x3, y3, z3);
  }

  //side faces
  int i = 0;
  for(; i < num_steps; i++)
  {
    int i1 = i;
    int i2 = (i + 1) % num_steps;
    int i3 = i + num_steps;

    //p1
    double x1 = points->m[0][i1];
    double y1 = points->m[1][i1];
    double z1 = points->m[2][i1];

    //p2
    double x2 = points->m[0][i2];
    double y2 = points->m[1][i2];
    double z2 = points->m[2][i2];

    //p3
    double x3 = points->m[0][i3];
    double y3 = points->m[1][i3];
    double z3 = points->m[2][i3];

    add_polygon(edges, x1, y1, z1, x2, y2, z2, x3, y3, z3);
  }

  i = 0;
  for(; i < num_steps; i++)
  {
    int i1 = i;
    int i2 = i + num_steps;
    int i3 = (i - 1 + num_steps) % num_steps + num_steps;

    //p1
    double x1 = points->m[0][i1];
    double y1 = points->m[1][i1];
    double z1 = points->m[2][i1];

    //p2
    double x2 = points->m[0][i2];
    double y2 = points->m[1][i2];
    double z2 = points->m[2][i2];

    //p3
    double x3 = points->m[0][i3];
    double y3 = points->m[1][i3];
    double z3 = points->m[2][i3];

    add_polygon(edges, x1, y1, z1, x2, y2, z2, x3, y3, z3);
  }

  free_matrix(points);

}

/*======== void add_sphere() ==========
  Inputs:   struct matrix * points
            double cx
	    double cy
	    double cz
	    double r
	    double step  
  Returns: 

  adds all the points for a sphere with center 
  (cx, cy, cz) and radius r.

  should call generate_sphere to create the
  necessary points
  ====================*/
void add_sphere( struct matrix * edges, double cx, double cy, double cz, double r, double step ) {

  struct matrix* points = generate_sphere(cx, cy, cz, r, step);
  int num_steps = (int) (1/step + 0.1);
  int lat, longt;
  int latStop, longStop, latStart, longStart;
  latStart = 0;
  latStop = num_steps;
  longStart = 0;
  longStop = num_steps;

  num_steps++;
  for(lat = latStart; lat < latStop; lat++)
  {
    for(longt = longStart; longt < longStop; longt++)
    {
      //There seem to be some extra edges near the pole - not sure how to resolve.

      //PART1
      int i1 = lat * num_steps + longt; i1 %= points->lastcol;
      int i2 = i1 + 1; i2 %= points->lastcol;
      int i3 = i1 + num_steps + 1; i3 %= points->lastcol;

      //p1
      double x1 = points->m[0][i1];
      double y1 = points->m[1][i1];
      double z1 = points->m[2][i1];

      //p2
      double x2 = points->m[0][i2];
      double y2 = points->m[1][i2];
      double z2 = points->m[2][i2];

      //p3
      double x3 = points->m[0][i3];
      double y3 = points->m[1][i3];
      double z3 = points->m[2][i3];

      add_polygon(edges, x1, y1, z1, x2, y2, z2, x3, y3, z3);

      //PART2
      i1 = lat * num_steps + longt; i1 %= points->lastcol;
      i2 = i1 + num_steps + 1; i2 %= points->lastcol;
      i3 = i1 + num_steps; i3 %= points->lastcol;

      //p1
      x1 = points->m[0][i1];
      y1 = points->m[1][i1];
      z1 = points->m[2][i1];

      //p2
      x2 = points->m[0][i2];
      y2 = points->m[1][i2];
      z2 = points->m[2][i2];

      //p3
      x3 = points->m[0][i3];
      y3 = points->m[1][i3];
      z3 = points->m[2][i3];

      add_polygon(edges, x1, y1, z1, x2, y2, z2, x3, y3, z3);
    }
  }
}

/*======== void generate_sphere() ==========
  Inputs:   double cx
	    double cy
	    double cz
	    double r
	    double step  
  Returns: Generates all the points along the surface 
           of a sphere with center (cx, cy, cz) and
	   radius r.
	   Returns a matrix of those points
  ====================*/
struct matrix * generate_sphere(double cx, double cy, double cz, double r, double step ) {

  int num_steps = (int)(1/step +0.1);
  
  struct matrix *points = new_matrix(4, num_steps * num_steps);
  int circle, rotation, rot_start, rot_stop, circ_start, circ_stop;
  double x, y, z, rot, circ;
  
  rot_start = 0;
  rot_stop = num_steps;
  circ_start = 0;
  circ_stop = num_steps;
  
  for (rotation = rot_start; rotation < rot_stop; rotation++) {
    rot = (double)rotation / num_steps;
    
    for(circle = circ_start; circle <= circ_stop; circle++){
      circ = (double)circle / num_steps;

      x = r * cos(M_PI * circ) + cx;
      y = r * sin(M_PI * circ) * cos(2*M_PI * rot) + cy;
      z = r * sin(M_PI * circ) * sin(2*M_PI * rot) + cz;
      
      // printf("rotation: %d\tcircle: %d\n", rotation, circle);
      // printf("rot: %lf\tcirc: %lf\n", rot, circ);
      // printf("sphere point: (%0.2f, %0.2f, %0.2f)\n\n", x, y, z);
      add_point(points, x, y, z);
    }
  }
  
  return points;
}

/*======== void add_torus() ==========
  Inputs:   struct matrix * points
            double cx
	    double cy
	    double cz
	    double r1
	    double r2
	    double step  
  Returns: 

  adds all the points required to make a torus
  with center (cx, cy, cz) and radii r1 and r2.

  should call generate_torus to create the
  necessary points
  ====================*/
void add_torus( struct matrix * edges, double cx, double cy, double cz, double r1, double r2, double step ) {
  
  struct matrix *points = generate_torus(cx, cy, cz, r1, r2, step);
  int num_steps = (int) (1/step + 0.1);
  int lat, longt;
  int latStop, longStop, latStart, longStart;
  latStart = 0;
  latStop = num_steps;
  longStart = 0;
  longStop = num_steps;

  for(lat = latStart; lat < latStop; lat++)
  {
    for(longt = longStart; longt < longStop; longt++)
    {
      //PART 1
      int i1 = lat * num_steps + longt;
      int i2 = (i1 + 1) % num_steps + lat * num_steps;
      int i3 = (i1 + num_steps + 1) % num_steps + (lat + 1) * num_steps; i3 %= points->lastcol;

      //p1
      double x1 = points->m[0][i1];
      double y1 = points->m[1][i1];
      double z1 = points->m[2][i1];

      //p2
      double x2 = points->m[0][i2];
      double y2 = points->m[1][i2];
      double z2 = points->m[2][i2];

      //p3
      double x3 = points->m[0][i3];
      double y3 = points->m[1][i3];
      double z3 = points->m[2][i3];

      add_polygon(edges, x1, y1, z1, x2, y2, z2, x3, y3, z3);

      //PART 2
      i1 = lat * num_steps + longt;
      i2 = (i1 + num_steps + 1) % num_steps + (lat + 1) * num_steps; i2 %= points->lastcol;
      i3 = (i1 + num_steps) % num_steps + (lat + 1) * num_steps; i3 %= points->lastcol;

      //p1
      x1 = points->m[0][i1];
      y1 = points->m[1][i1];
      z1 = points->m[2][i1];

      //p2
      x2 = points->m[0][i2];
      y2 = points->m[1][i2];
      z2 = points->m[2][i2];

      //p3
      x3 = points->m[0][i3];
      y3 = points->m[1][i3];
      z3 = points->m[2][i3];

      add_polygon(edges, x1, y1, z1, x2, y2, z2, x3, y3, z3);
    }
  }

  free_matrix(points);
}


/*======== void generate_torus() ==========
  Inputs:   struct matrix * points
            double cx
	    double cy
	    double cz
	    double r
	    double step  
  Returns: Generates all the points along the surface 
           of a torus with center (cx, cy, cz) and
	   radii r1 and r2.
	   Returns a matrix of those points
  ====================*/
struct matrix * generate_torus( double cx, double cy, double cz, double r1, double r2, double step ) {
  int num_steps = (int)(1/step +0.1);
  
  struct matrix *points = new_matrix(4, num_steps * num_steps);
  int circle, rotation, rot_start, rot_stop, circ_start, circ_stop;
  double x, y, z, rot, circ;

  rot_start = 0;
  rot_stop = num_steps;
  circ_start = 0;
  circ_stop = num_steps;
  
  for (rotation = rot_start; rotation < rot_stop; rotation++) {
    rot = (double)rotation / num_steps;
    
    for(circle = circ_start; circle < circ_stop; circle++){
      circ = (double)circle / num_steps;

      x = cos(2*M_PI * rot) * (r1 * cos(2*M_PI * circ) + r2) + cx;
      y = r1 * sin(2*M_PI * circ) + cy;
      z = -1*sin(2*M_PI * rot) * (r1 * cos(2*M_PI * circ) + r2) + cz;
      
      //printf("rotation: %d\tcircle: %d\n", rotation, circle);
      //printf("torus point: (%0.2f, %0.2f, %0.2f)\n", x, y, z);
      add_point(points, x, y, z);
    }
  }
  
  return points;
}

/*======== void add_circle() ==========
  Inputs:   struct matrix * points
            double cx
	    double cy
	    double r
	    double step  
  Returns: 

  Adds the circle at (cx, cy) with radius r to edges
  ====================*/
void add_circle( struct matrix * edges, double cx, double cy, double cz, double r, double step ) {
  
  double x0, y0, x1, y1, t;

  x0 = r + cx;
  y0 = cy;
  for (t=step; t <= 1.00001; t+= step) {
    
    x1 = r * cos(2 * M_PI * t) + cx;
    y1 = r * sin(2 * M_PI * t) + cy;

    add_edge(edges, x0, y0, cz, x1, y1, cz);
    x0 = x1;
    y0 = y1;
  }
}

/*======== void add_curve() ==========
Inputs:   struct matrix *points
         double x0
         double y0
         double x1
         double y1
         double x2
         double y2
         double x3
         double y3
         double step
         int type  
Returns: 

Adds the curve bounded by the 4 points passsed as parameters
of type specified in type (see matrix.h for curve type constants)
to the matrix points
====================*/
void add_curve( struct matrix *edges, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, double step, int type ) {

  double t, x, y; 
  struct matrix *xcoefs;
  struct matrix *ycoefs;

  xcoefs = generate_curve_coefs(x0, x1, x2, x3, type);
  ycoefs = generate_curve_coefs(y0, y1, y2, y3, type);
  
  /* print_matrix(xcoefs); */
  /* printf("\n"); */
  /* print_matrix(ycoefs); */
  
  for (t=step; t <= 1.000001; t+= step) {

    x = xcoefs->m[0][0] *t*t*t + xcoefs->m[1][0] *t*t+
      xcoefs->m[2][0] *t + xcoefs->m[3][0];
    y = ycoefs->m[0][0] *t*t*t + ycoefs->m[1][0] *t*t+
      ycoefs->m[2][0] *t + ycoefs->m[3][0];
    
    add_edge(edges, x0, y0, 0, x, y, 0);
    x0 = x;
    y0 = y;
  }
  
  free_matrix(xcoefs);
  free_matrix(ycoefs);
}


/*======== void add_point() ==========
Inputs:   struct matrix * points
         int x
         int y
         int z 
Returns: 
adds point (x, y, z) to points and increment points.lastcol
if points is full, should call grow on points
====================*/
void add_point( struct matrix * points, double x, double y, double z) {

  if ( points->lastcol == points->cols )
    grow_matrix( points, points->lastcol + 100 );
  
  points->m[0][ points->lastcol ] = x;
  points->m[1][ points->lastcol ] = y;
  points->m[2][ points->lastcol ] = z;
  points->m[3][ points->lastcol ] = 1;
  points->lastcol++;
} //end add_point

/*======== void add_edge() ==========
Inputs:   struct matrix * points
          int x0, int y0, int z0, int x1, int y1, int z1
Returns: 
add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
should use add_point
====================*/
void add_edge( struct matrix * points, double x0, double y0, double z0, double x1, double y1, double z1) {
  add_point( points, x0, y0, z0 );
  add_point( points, x1, y1, z1 );
}

/*======== void draw_lines() ==========
Inputs:   struct matrix * points
         screen s
         color c 
Returns: 
Go through points 2 at a time and call draw_line to add that line
to the screen
====================*/
void draw_lines( struct matrix * points, screen s, color c) {

 if ( points->lastcol < 2 ) {
   printf("Need at least 2 points to draw a line!\n");
   return;
 }
 
 int point;
 for (point=0; point < points->lastcol-1; point+=2)
   draw_line( points->m[0][point],
	      points->m[1][point],
	      points->m[0][point+1],
	      points->m[1][point+1],
	      s, c);	       
}// end draw_lines









void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {
  
  int x, y, d, A, B;
  //swap points if going right -> left
  int xt, yt;
  if (x0 > x1) {
    xt = x0;
    yt = y0;
    x0 = x1;
    y0 = y1;
    x1 = xt;
    y1 = yt;
  }
  
  x = x0;
  y = y0;
  A = 2 * (y1 - y0);
  B = -2 * (x1 - x0);  

  //octants 1 and 8
  if ( abs(x1 - x0) >= abs(y1 - y0) ) {

    //octant 1    
    if ( A > 0 ) {
      
      d = A + B/2;      
      while ( x < x1 ) {
	plot( s, c, x, y );
	if ( d > 0 ) {
	  y+= 1;
	  d+= B;
	}
	x++;
	d+= A;
      } //end octant 1 while
      plot( s, c, x1, y1 );
    } //end octant 1

    //octant 8
    else {
      d = A - B/2;
      
      while ( x < x1 ) {
	//printf("(%d, %d)\n", x, y);
	plot( s, c, x, y );
	if ( d < 0 ) {
	  y-= 1;
	  d-= B;
	}
	x++;
	d+= A;
      } //end octant 8 while
      plot( s, c, x1, y1 );
    } //end octant 8
  }//end octants 1 and 8

  //octants 2 and 7
  else {
    
    //octant 2    
    if ( A > 0 ) {
      d = A/2 + B;      

      while ( y < y1 ) {
	plot( s, c, x, y );
	if ( d < 0 ) {
	  x+= 1;
	  d+= A;
	}
	y++;
	d+= B;
      } //end octant 2 while
      plot( s, c, x1, y1 );
    } //end octant 2

    //octant 7
    else {
      d = A/2 - B;
      
      while ( y > y1 ) {
	plot( s, c, x, y );
	if ( d > 0 ) {
	  x+= 1;
	  d+= A;
	}
	y--;
	d-= B;
      } //end octant 7 while
      plot( s, c, x1, y1 );
    } //end octant 7   
  }//end octants 2 and 7  
} //end draw_line