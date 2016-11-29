#include "Shot.h"

Shot::Shot (){
	this->r = 1;
	this->g = 1;
	this->b = 0;
	this->size = 10;
	this->x = 0;
	this->y = 0;
	this->theta = 0;
	this->distance = 500;
	this->speed = 0;
}

void Shot::set_color(float r, float g, float b){
	this->r = r;
	this->g = g;
	this->b = b;
}
void Shot::set_size(float size){
	this->size = size;
}
void Shot::set_speed(float speed){
	this->speed = speed;
}
void Shot::set_max_distance(float distance){
	this->distance = distance;
}
void Shot::set_pos(float x, float y, float theta){
	this->x = x;
	this->y = y;
	this->theta = theta;
}
bool Shot::refresh_pos(float delta_t){
	this->x = this->x - delta_t*this->speed*sin(this->theta*M_PI/180);
	this->y = this->y + delta_t*this->speed*cos(this->theta*M_PI/180);
	if ((this->distance -= this->speed*delta_t) < 0)
		return false;
	return true;
}
void Shot::draw(void){
	glPushMatrix();
		glTranslatef(this->x,this->y,0);	
		glRotatef(this->theta,0,0,1);
		draw_circ(this->size,this->r,this->g,this->b,10);
		glTranslatef(0,-this->size/2,0);
		draw_rect(this->size*2,this->size,this->r,this->g,this->b);
	glPopMatrix();
}

