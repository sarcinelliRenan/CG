#include "GameObject.h"

bool GameObject::colide (GameObject* obj){
	float distance = sqrt(pow(obj->x-this->x,2)+pow(obj->y-this->y,2));
	return (distance <= obj->size/2 + this->size/2);
}
