#include "myObject.h"
#include "sprite.h"
#include "levelMap.h"
#include "player_limits.h"
struct levelMap;

SDL_Renderer* renderer = NULL;
bool quit = false;
myObject objectDot;
levelMap* firstLevel = NULL; 

//Should only have a single instance
playerManager pmanager(&objectDot);

void
eventHandler(SDL_Event& e){
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	pmanager.keyStateCheck(keystate);

	while( SDL_PollEvent( &e ) != 0 ){

		if(!pmanager.keyPressCheck(e)){
			if( e.type == SDL_QUIT ){
				quit = true;
			}
		}
	}
}
