#ifndef STD_DRAW_H
#define STD_DRAW_H

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define DEG2RAD 3.14159/180

void draw_circ(float radius,float r,float g,float b,float points);

void draw_rect(float height, float width,float r,float g,float b);
//TC2 still
typedef struct{
	float cx,cy; 		//coordinates of the center
	float radius;		//radius of the circle
	float r,g,b;		//color
}Circle;

void draw_circle (Circle circ);

typedef struct{
	float x,y; 		//rect base
	float h,w;		//rect dimensions
	float r,g,b;	//color
}Rectangle;

void draw_rectangle (Rectangle rect);

GLuint LoadTextureRAW( const char * filename );

void draw_ground(Circle lane,GLuint gndTex);
void draw_inner_wall(Circle circ,GLuint wallTex, float height);
void draw_out_wall(Circle circ,GLuint wallTex, float height);
void draw_start_mark(Rectangle rect,GLuint strTex);
#endif
