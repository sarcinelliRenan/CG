#ifndef CAR_H
#define CAR_H

#include <GL/glut.h>
#include <stdio.h>
#include "std_draw.h"
#include "Shot.h"
#include "GameObject.h"

extern GLuint carTex;
extern GLuint gunTex;

class Car : public GameObject{
	float wheel_angle;
	float cannon_angle;
	float wheel_sprite;
	float r,g,b;
	float car_speed,shot_speed,rotate_speed;
	double fire_flag;

	void body (float r,float g,float b);
	void wheel ();
	void cannon(float r,float g,float b);

	public:

		Car (float r,float g,float b,float size);
		Car ();
		void set_color(float r, float g, float b);
		void set_size(float size);
		void draw(void);
		void inc_cannon_angle(float angle);
		void set_cannon_angle(float angle);
		void left(float delta_t);
		void right(float delta_t);
		void forward(float delta_t);
		void back(float delta_t);	
		void set_pos(float x,float y, float tetha);
		bool on_lane(Circle lane_in, Circle lane_out);

		void set_speed(float speed);
		void set_bullet_speed(float speed);

		Shot fire ();

		float get_x();
		float get_y();
		float get_theta();
		float get_size();
};	

#endif
