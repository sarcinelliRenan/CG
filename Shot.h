#ifndef SHOT_H
#define SHOT_H

#include <GL/glut.h>
#include <stdio.h>
#include "std_draw.h"
#include "GameObject.h"

class Shot : public GameObject{
	float r,g,b;
	float distance;
	float speed;

	public:
		Shot ();
		void set_color(float r, float g, float b);
		void set_size(float size);
		void draw(void);
		void set_pos(float x,float y, float tetha);
		bool refresh_pos(float delta_t);
		void set_speed(float speed);
		void set_max_distance(float distance);
		
};	

#endif
