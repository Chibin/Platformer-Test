#include "myObject.h"
#include "sprite.h"
#include "levelMap.h"
#include "player_limits.h"
struct levelMap;

bool quit = false;
myObject objectDot;
levelMap* firstLevel = new levelMap(80,80,"39_tiling/tiles.png");

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
