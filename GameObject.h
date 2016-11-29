#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <math.h>

class GameObject {
	protected:
		float size;
		float x,y,theta;	

	public:
	
		bool colide(GameObject* obj);
};

#endif
