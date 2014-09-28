//Limitations to what a player can do
//Keeps track of states


struct playerManager{
	myObject* _player;
	Uint32 ticks_up;
	Uint32 ticks_down;
	Uint32 ticks_left;
	Uint32 ticks_right;
	Uint32 ticks_jump; 
	playerManager(myObject*);	
	
	// Checks if the button is still being pressed
	// And updates the position of the player accordingly
	void keyStateCheck(const Uint8*);
	
	// Checks for new key events
	// returns true if the event type is a key related
	bool keyPressCheck(const SDL_Event);
};

playerManager::playerManager(myObject* player){
	_player = player;
	ticks_up = ticks_down = ticks_left = ticks_right = 0;
	ticks_jump = 0;
}

bool
playerManager::keyPressCheck(const SDL_Event e){
	bool isKey = false;

	const float velMargin = -7.7f; // Margin for velocity to be acceptable
	if( e.type == SDL_KEYDOWN ){
		SDL_Keycode keyPressed = e.key.keysym.sym;
		switch(keyPressed){
			case SDLK_UP:
				if( !e.key.repeat 
					&& _player->jumpCounter < _player->jumpLimit
					&& _player->accely == 0.0f && _player->vely >= velMargin ){

					//printf("Accel : %f\n", _player->accely);
					_player->setVelY(70.0f);
					_player->setAccelY(300.0f);
					_player->jumpCounter++;
				}
				break;
			case SDLK_DOWN:
				break;
			case SDLK_LEFT:
				if( !e.key.repeat ){
					if( SDL_GetTicks()  - ticks_left < 200 ){
						_player->isDashX = true;
						if( (!_player->accely && _player->vely > velMargin && _player->vely > 0.0f)
							|| (_player->vely < velMargin && !_player->accely) )
						{
							//printf("Accel : %f\n", _player->accely);
							//printf("Vel : %f\n", _player->vely);
							_player->setVelX(-1000.0f);
							_player->setAccelX(-50000.0f);
							_player->setAccelY(0.0f);
							_player->setVelY(0.0f);
						}
					}
					else{
						ticks_left = SDL_GetTicks();
						_player->isDashX = false;
					}
				}
				break;
			case SDLK_RIGHT:
				if( !e.key.repeat ){
					if( SDL_GetTicks()  - ticks_right < 200 ){
						_player->isDashX = true;
							//printf("Vel : %f\n", _player->vely);
						if( (!_player->accely && _player->vely > velMargin && _player->vely > 0.0f)
							|| (_player->vely < velMargin && !_player->accely) )
						{
							//printf("Accel : %f\n", _player->accely);
							//printf("Vel : %f\n", _player->vely);
							_player->setVelX(1000.0f);
							_player->setAccelX(50000.0f);
							_player->setAccelY(0.0f);
							_player->setVelY(0.0f);
						}
					}
					else{
						ticks_right = SDL_GetTicks();
						_player->isDashX = false;
					}
				}
				break;
			default:
				printf("Pressing other buttons ..\n");
				break;
		}
		isKey = true;
	}

	else if( e.type == SDL_KEYUP ){
		SDL_Keycode keyPressed = e.key.keysym.sym;
		switch(keyPressed){
			case SDLK_UP:
				break;
			case SDLK_DOWN:
				break;
			case SDLK_LEFT:
				break;
			case SDLK_RIGHT:
				break;
			default:
				break;
		}
		isKey = true;
	}

	return isKey;
}

void
playerManager::keyStateCheck(const Uint8* keystate){
	float dashModX = 1.5;
	if(keystate[SDL_SCANCODE_UP]){
			//_player->setVelY(9.81);
	}
	if(keystate[SDL_SCANCODE_DOWN]){
		_player->setAccelY(-40.81f);
	}
	if(keystate[SDL_SCANCODE_LEFT]){
		if( _player->isDashX ){
			_player->setVelX(-40.81f*dashModX);
		}
		else{
			_player->setVelX(-40.81f);
		}
	}
	if(keystate[SDL_SCANCODE_RIGHT]){
		if( _player->isDashX ){
			_player->setVelX(40.81f*dashModX);
		}
		else{
			_player->setVelX(40.81f);
		}
	}
	if(keystate[SDL_SCANCODE_SPACE]){
		_player->setPos(250,0);
	}
	if(keystate[SDL_SCANCODE_Z]){
		_player->setVel(0,0);
	}
}
