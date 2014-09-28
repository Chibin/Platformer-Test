struct myObject;
std::vector<myObject*> movingObjects;
unsigned int lastTime = 0, currentTime, deltaTime;

#define GRAVITY -9.80665

void
position_equation(myObject* object, unsigned int ms){
	float seconds = (float)ms/1000;
	//printf("Delta time : %f\n", (float)ms/1000);
	float delta_y = 0.0, delta_x = 0.0;
	int stored_x, stored_y;
	stored_x = object->getRect()->x;
	stored_y = object->getRect()->y;
	//Reverse directions
	delta_y = (float)(-1 * object->vely * seconds
		+ -1
		* (0.5) 
		* (object->accely+GRAVITY*object->_weight)
		* seconds * seconds);
	delta_x = (float)object->velx * seconds + 
		(float)(0.5)*(object->accelx)*seconds*seconds;

	if( object->hasAccelY() ){
		object->setAccelY( object->accely*0.001);
	}
	if( object->hasAccelX() ){
		object->setAccelX( object->accelx*0.001);
	}
	if( object->velx != 0.0 ){
		object->velx *= 0.9;
	}
	object->X += delta_x;
	object->Y += delta_y;
	object->addStoredPosX();
	//Seprate collissions for X and Y
	if( firstLevel->collissionManager(object) ){
		object->setPosX(stored_x);
		object->setVelX(0.0f);
		// May need to do some kind of object collission check
	}

	object->addStoredPosY();
	if( firstLevel->collissionManager(object) ){
		object->setPosY(stored_y);
		object->setVelY(0.0f);
		object->jumpCounter = 0;
	}
	else{
		object->addVelY(
					(float)
					(object->accely + GRAVITY * object->_weight)
					* seconds);
	}

}

void
physics(){
	
	for(int i = 0; i < movingObjects.size(); i++){
		//May need to be moved to levelMap
		position_equation(movingObjects[i], deltaTime);
	//	printf("%d\n", movingObjects[i]->Pos.y);
	}

}
