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
	float z;
	float phi;

	public:
		Shot ();
		void set_color(float r, float g, float b);
		void set_size(float size);
		void draw(void);
		void set_pos(float x,float y, float tetha);
		void set_pos3d(float x, float y, float z, float theta, float phi);
		void set_phi(float phi);
		bool refresh_pos(float delta_t);
		void set_speed(float speed);
		void set_max_distance(float distance);

};

#endif
