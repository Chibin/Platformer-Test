#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <Queue>
#include <map>
#include <unordered_map>
#include <list>

#include "SDL_image.h"
#include "eventHandler.h"
#include "physics.h"
#include "mapHandler.h"
#include "textureWrapper.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//window for rendering
SDL_Window* window = NULL;

//Surface contained by the window
SDL_Surface* screenSurface = NULL;

std::vector<SDL_Texture*> sdl_texture_list;
std::vector<SDL_Surface*> sdl_surface_list;
std::vector<SDL_Rect*> sdl_rect_list;

// Moved to texture wrapper
SDL_Texture* gTexture = NULL;

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


struct position{
	public:
		int x, y, cost;
		position& operator=(const position& a){
			x=a.x;
			y=a.y;
			cost = a.cost;
			return *this;
		}

		bool operator==(const position& a) const{
			return (x == a.x && y == a.y);
		}

		bool operator!=(const position& a) const{
			return (x != a.x || y != a.y);
		}

		friend std::ostream& operator<<( std::ostream& stream,
				const position& pos){
			stream << "Position: " << pos.x << " " << pos.y;
			return stream;
		}
};

struct costComparator{
	bool operator()(const position& left, const position& right){
		return left.cost < right.cost;
	}
};

struct posComparator{
	bool operator()(const position& left, const position& right){
		if ( left.x < right.x){
			return true;
		}
		else if( left.x == right.x){
			if ( left.y < right.y){
				return true;
			}
			else if( left.y == right.y){
				return false;
			}
		}
		return false;
	}
};

bool init(){

	//init SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf("SDL not initialized! \n SDL_ERROR: \
				%s\n", SDL_GetError() );
		return false;
	}
	else {
		if( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") ){
			printf("Warning: linear texture filter not enabled.");
		}
		
		//create window
		window = SDL_CreateWindow ( "Testing",	
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									SCREEN_WIDTH,
									SCREEN_HEIGHT,
									SDL_WINDOW_SHOWN );
		if( window == NULL ){
			printf( "Window could not be created! \n \
					SDL_ERROR: %s\n", SDL_GetError() );
			return false;
		}
		else{
			// Vsync causes the refresh rate of the window to be the same as the refresh rate of the monitor
			// May cause a slow down due to the refresh rate of the monitor, however, there will be no image
			// tears during the refresh.
			// SDL_RENDERER_PRESENTVSYNC
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
			if( renderer == NULL){
				printf("Renderer could not be craeted! SDL Error: %s\n", SDL_GetError() );
			}
			else{
				//init render color
				SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
				firstLevel = new levelMap(80,80,"39_tiling/tiles.png", renderer);

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					return false;
				}
			}
			//Window surface
			//screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return true;
}

//Update the surface
void update(SDL_Window* object){
	// Erase everything in the screen

	SDL_UpdateWindowSurface(object);
}

void loadMedia(const char* file, struct position pos, SDL_Rect* _rect = NULL){
	SDL_Surface* bmp_surface = NULL;
	SDL_Texture* newTexture = NULL;
	//load image
	bmp_surface = SDL_LoadBMP(file);
	if(bmp_surface == NULL){
		printf( "Unable to load image %s! \n\
				SDL Error: %s\n", file, SDL_GetError() );
	}
	else{
		printf("Loaded successfully: %s\n",file);
		newTexture = SDL_CreateTextureFromSurface(renderer, bmp_surface);
		sdl_texture_list.push_back(newTexture);
		std::cout << sdl_texture_list[0] << std::endl;
		sdl_surface_list.push_back(bmp_surface);
		if ( _rect != NULL )
			sdl_rect_list.push_back(_rect);
		else{
			SDL_Rect* rect = new SDL_Rect();
			rect->x = pos.x;
			rect->y = pos.y;
			sdl_rect_list.push_back(rect);
		}
	}
}

void close(){
	// Have surface list?

	// Kill the window afterwards

	// quit
}

std::list<position> find_path( position start, position goal, char*** gridMap, 
		std::map<std::string, std::string> &isMarked
		){

	std::priority_queue<position, std::vector<position>, costComparator> testing;
	testing.push(start);
	std::map<position, position, posComparator> came_from;
	std::map<position, uint16_t, posComparator> cost_so_far;
	std::list<position> solution;

	position current;
	came_from[current] = current;
	cost_so_far[current] = 0;
	while( !testing.empty() ){
		current = testing.top();
		testing.pop();

		if( current == goal){
			position prev = current;
			printf("GOAL FOUND\n");
			std::cout << prev << std::endl;
			solution.push_front(prev);
			while(came_from[prev] != start){
				std::cout << came_from[prev] << std::endl;
				prev = came_from[prev];
				solution.push_front(prev);
			}
			break;
		}

		position next;
		next.x = current.x; next.y = current.y-1;

		if( next.x < 0 || next.y < 0 || next.y > 5 || next.x > 7 || 
				came_from.find(next) != came_from.end() ||
				gridMap[next.x][next.y][0] == '0' && 
				gridMap[next.x][next.y][1] == '0' ){
		}
		else{
			testing.push(next);
			came_from[next] = current;
			std::cout << "Push "<< next << std::endl;
		}

		//down
		next.x = current.x; next.y = current.y+1;
		if( next.x < 0 || next.y < 0 || next.y > 5 || next.x > 7 || 
				came_from.find(next) != came_from.end() ||
				gridMap[next.x][next.y][0] == '0' && 
				gridMap[next.x][next.y][1] == '0' ){
		}
		else{
			testing.push(next);
			came_from[next] = current;
			std::cout << "Push "<< next << " " << current << std::endl;
		}

		//left
		next.x = current.x-1; next.y = current.y;
		if( next.x < 0 || next.y < 0 || next.y > 5 || next.x > 7 || 
				came_from.find(next) != came_from.end() ||
				gridMap[next.x][next.y][0] == '0' && 
				gridMap[next.x][next.y][1] == '0' ){
		}
		else{
			testing.push(next);
			came_from[next] = current;
			std::cout << "Push "<< next << " " << current << std::endl;
		}

		//right
		next.x = current.x+1; next.y = current.y;
		if( next.x < 0 || next.y < 0 || next.y > 5 || next.x > 7 || 
				came_from.find(next) != came_from.end() ||
				gridMap[next.x][next.y][0] == '0' && 
				gridMap[next.x][next.y][1] == '0' ){
		}
		else{
			testing.push(next);
			came_from[next] = current;
			std::cout << "Push "<< next << " " << current << std::endl;
		}
	}
	return solution;
}

int main( int argc, char* args[] )
{
	// Event stuff...
	SDL_Event e;

	objectDot.setPos(250,0);
	// Dot Player currently a global
	SDL_Rect* dotPos = objectDot.getRect();

	movingObjects.push_back(&objectDot);

	SDL_Rect* aiPos = new SDL_Rect();
	aiPos->x = 40; aiPos->y = 40;


	position start, goal;
	start.x = 0; start.y = 0; start.cost = 50;
	goal.x = 7; goal.y = 0;
	std::map<std::string, std::string> isMarked;
	std::list<position> checkSol;
	//checkSol = find_path(start,goal, gridMap, isMarked);
	bool inProgress = false, startpath = false;
	if( !checkSol.empty() ){
		startpath = true;	
	}

	if( !init() ){
		return 1;
	}
	else{
		//The image we will load and show on the screen 
		position pos;
		//pos.x = 0; pos.y = 0;
		//loadMedia("x.bmp", pos);
		pos.x = 20; pos.y = 20;
		// mapHandler.h
		loadStage(renderer); //Loads all the necessary map info

		objectDot.loadMedia("39_tiling/dot.bmp", renderer);
		sdl_surface_list.push_back(objectDot.getSpriteSurface());
		sdl_texture_list.push_back(objectDot.getSpriteTexture());
		sdl_rect_list.push_back(dotPos);

		for( int i = 0; i < sdl_surface_list.size(); i++){
			if( sdl_surface_list[i] == NULL ){
				printf("index: %d is NULL", i);
			}
		}
		for( int i = 0; i < sdl_texture_list.size(); i++){
			if( sdl_texture_list[i] == NULL ){
				printf("index: %d is NULL", i);
			}
		}
	}

	// creaaazy main loop
	int counter = 0;
	lastTime = SDL_GetTicks();
	while( !quit ){

		eventHandler(e);


		//Move the AI
		//if( startpath ){
		//	position temp = checkSol.front();
		//	if(counter > 5){
		//		if( aiPos->x == temp.x*80+40){
		//		}
		//		else if( aiPos->x < temp.x*80+40) aiPos->x++;
		//		else aiPos->x--;

		//		if( aiPos->y == temp.y*80+40){
		//		}
		//		else if( aiPos->y < temp.y*80+40) aiPos->y++;
		//		else aiPos->y--;

		//		if( aiPos->x == temp.x*80+40 && 
		//			aiPos->y == temp.y*80+40 ){
		//			std::cout << temp << std::endl;
		//			if(checkSol.size() <= 1) startpath = false; 
		//			else checkSol.pop_front();
		//		}
		//		counter = 0;
		//	}
		//}
		//else{
		//	startpath = false;
		//}
		//counter++;

		currentTime = SDL_GetTicks();
		deltaTime  = currentTime - lastTime;
		while( deltaTime < (double)1000/60 ){
			currentTime = SDL_GetTicks();
			deltaTime  += currentTime - lastTime;
		}
		lastTime = currentTime;
		
		// Do physics here
		physics();

		//SDL_FillRect(screenSurface, NULL, 0x000000);
		// clean the screen first
		SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
		SDL_RenderClear( renderer );
		firstLevel->drawMap(renderer, dotPos);

		for( int i = 0; i < sdl_surface_list.size(); i++){
			//SDL_BlitSurface( sdl_surface_list[i], NULL, screenSurface, sdl_rect_list[i] );
			int x, y;
			x = sdl_rect_list[i]->x;
			y = sdl_rect_list[i]->y;

			if( sdl_rect_list[i]->x > SCREEN_WIDTH/2 ){
				if( SCREEN_WIDTH/2 > firstLevel->LEVEL_WIDTH - x ){
					x = SCREEN_WIDTH - (firstLevel->LEVEL_WIDTH - x);
				}
				else{
					x = SCREEN_WIDTH/2;
				}
				printf("x: %d\n", x);
			}
			if( sdl_rect_list[i]->y > SCREEN_HEIGHT ){
				y = sdl_rect_list[i]->y - SCREEN_HEIGHT;
			}

			SDL_Rect temp = {x, y, 20, 20};
			SDL_RenderCopy( renderer, sdl_texture_list[i], NULL , &temp);
		}
		
		//SDL_BlitSurface( sdl_surface_list[0], NULL, screenSurface, aiPos );
		//update( window );
		SDL_RenderPresent(renderer);
		//SDL_PumpEvents();
	}

	SDL_DestroyWindow(window);
	SDL_Quit();


	return 0;
}
