struct myObject{
	SDL_Rect Pos;
	SDL_Surface* sprite;
	float _weight;
	float X, Y; // Tentative Positions
	float velx, vely;
	float accelx, accely;
	bool isDashX, isDashY;
	short jumpLimit;
	short jumpCounter;
	myObject();
	void setPos(int, int);
	void setPosY(int);
	void setPosX(int);
	void addPos(int, int);
	void addStoredPos();
	void addStoredPosX();
	void addStoredPosY();
	SDL_Rect* getRect();
	void setVel(float, float);
	void setVelX(float);
	void setVelY(float);
	void setAccel(float,float);
	void setAccelX(float);
	void setAccelY(float);
	void addVelY(float);
	void addVel(float,float);
	bool hasAccelX();
	bool hasAccelY();
	// Will need to create a hash map of all the loaded textures later on
	void loadMedia(const char*);
	SDL_Surface* getSpriteSurface();
};

myObject::myObject(){
	velx = vely = 0;
	_weight = 3;
	accelx = accely = 0;
	sprite = NULL;
	isDashX = isDashY = 0;
	jumpLimit = 1;
	jumpCounter = 0;
}

bool
myObject::hasAccelX(){
	return (accelx	!= 0.0f);
}

bool
myObject::hasAccelY(){
	return (accely	!= 0.0f);
}


SDL_Surface*
myObject::getSpriteSurface(){
	return sprite;
}

void 
myObject::loadMedia(const char* file){
	sprite = SDL_LoadBMP(file);
	if(sprite == NULL){
		printf( "Unable to load image %s! \n\
				SDL Error: %s\n", file, SDL_GetError() );
	}
}

void 
myObject::addVel(float _velx, float _vely){
	velx += _velx;
	vely += _vely;
}

void 
myObject::addVelY(float _vely){
	vely += _vely;
}

void 
myObject::setVel(float _velx, float _vely){
	velx = _velx;
	vely = _vely;
}

void 
myObject::setVelY(float _vely){
	vely = _vely;
}

void 
myObject::setVelX(float _velx){
	velx = _velx;
}

void 
myObject::setAccel(float _accelx, float _accely){
	accelx = _accelx;
	accely = _accely;
}

void 
myObject::setAccelX(float _accelx){
	accelx = _accelx;
}

void 
myObject::setAccelY(float _accely){
	accely = _accely;
}

void
myObject::addPos(int x, int y){
	Pos.x += x;
	Pos.y += y;
}

void
myObject::setPosX(int x){
	Pos.x = x;
	X = 0.0f;
}

void
myObject::setPosY(int y){
	Pos.y = y;
	Y = 0.0f;
}

void
myObject::setPos(int x, int y){
	Pos.x = x;
	Pos.y = y;
	X = 0.0f;
	Y = 0.0f;
}

void
myObject::addStoredPos(){
	Pos.x = Pos.x + (int)X;
	Pos.y = Pos.y + (int)Y;
	X = X - (int)X;
	Y = Y - (int)Y;
}

void
myObject::addStoredPosX(){
	Pos.x = Pos.x + (int)X;
	X = X - (int)X;
}

void
myObject::addStoredPosY(){
	Pos.y = Pos.y + (int)Y;
	Y = Y - (int)Y;
}

SDL_Rect*
myObject::getRect(){
	return &Pos;
}

