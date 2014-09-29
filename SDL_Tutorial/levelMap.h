struct levelMap{

	short tile_h, tile_w;
	short dim_x, dim_y;
	char*** gridMap;
	std::vector< std::pair<char*,SDL_Rect*> > map_rect;
	std::map< std::string, char > walls;
	std::vector<sprite> tileList;
	std::ifstream gridmap;
	SDL_Surface* tiles;
	SDL_Texture* texture_tiles;
	SDL_Renderer* renderer;
	levelMap(short, short, const char *, SDL_Renderer*);
	void loadMap(const char*);
	void populateMap();
	void drawMap(SDL_Renderer*);
	bool collissionManager(myObject* object);
	bool isCollideX(myObject* object);
	bool isCollideY(myObject* object);
	bool lrCheck(myObject* object);
	void collissionCheck();
	void populateCollissionMap();
	void printWalls();
	std::string wallCoord(int, int);
};

bool
levelMap::lrCheck(myObject* object){
	SDL_Rect* tentativePos = object->getRect();
	SDL_Surface* spriteinfo = object->getSpriteSurface();
	uint16_t t_leftx, t_rightx, b_leftx, b_rightx;
	uint16_t t_lefty, t_righty, b_lefty, b_righty;
	bool tl, tr, bl, br;
	tl, tr, bl, br = false;
	

	t_leftx = tentativePos->x; t_lefty = tentativePos->y;	
	t_rightx = tentativePos->x+spriteinfo->w; t_righty = tentativePos->y;
	b_leftx = tentativePos->x; b_lefty = tentativePos->y+spriteinfo->h;	

	b_rightx = tentativePos->x+spriteinfo->w; 
	b_righty = tentativePos->y+spriteinfo->h;	

	return true;
}

levelMap::levelMap(short tile_height, short tile_width,
				   const char* tile_path, SDL_Renderer* renderer){
	std::cout << renderer << std::endl;
	tiles = IMG_Load(tile_path);
	if(tiles == NULL){
		printf("Fail to load tiles.\n");
	}
	SDL_SetColorKey(tiles, SDL_TRUE, SDL_MapRGB( tiles->format, 0, 0xFF, 0xFF));
	texture_tiles = SDL_CreateTextureFromSurface( renderer, tiles );
	if ( texture_tiles == NULL){
		printf("load failure! SDL error: %s\n", SDL_GetError());
		std::cout << texture_tiles << std::endl;
	}
	char*** gridMap = NULL;
	tile_h = tile_height;
	tile_w = tile_width;
	walls.clear();
}

void
levelMap::printWalls(){
	std::map< std::string, char >::iterator it;
	for( std::map<std::string,char>::iterator it=walls.begin(); 
			it != walls.end(); it++){
		std::cout << it->first << " => " << it->second << '\n';
	}

}

void
levelMap::populateCollissionMap(){
	// May be for later on when more properties are created ...
}

void
levelMap::collissionCheck(){

}

std::string
levelMap::wallCoord(int x, int y){
	return std::to_string((long double)x) + 
			"," + std::to_string((long double)y);
}


bool
levelMap::isCollideX(myObject* object){
	SDL_Rect* tentativePos = object->getRect();
	SDL_Surface* spriteinfo = object->getSpriteSurface();
	uint16_t t_leftx, t_rightx, b_leftx, b_rightx;
	uint16_t t_lefty, t_righty, b_lefty, b_righty;
	bool tl, tr, bl, br;
	tl, tr, bl, br = false;

	t_leftx = tentativePos->x; t_lefty = tentativePos->y;	
	t_rightx = tentativePos->x+spriteinfo->w; t_righty = tentativePos->y;
	b_leftx = tentativePos->x; b_lefty = tentativePos->y+spriteinfo->h;	

	b_rightx = tentativePos->x+spriteinfo->w; 
	b_righty = tentativePos->y+spriteinfo->h;	

	if(walls.find(wallCoord(t_leftx/tile_w, t_lefty/tile_h)) 
			!= walls.end() && 
			walls.find(wallCoord(t_rightx/tile_w, t_righty/tile_h)) 
				== walls.end()) {
		//printf("X left\n");
		return true;
	}


	if(walls.find(wallCoord(t_rightx/tile_w, t_righty/tile_h)) 
			!= walls.end() && 
			walls.find(wallCoord(b_rightx/tile_w, b_righty/tile_h)) 
				!= walls.end())
	{
		//printf("X TOP right\n");
		return true;
	}
	return false;
}

bool
levelMap::isCollideY(myObject* object){
	SDL_Rect* tentativePos = object->getRect();
	SDL_Surface* spriteinfo = object->getSpriteSurface();
	uint16_t t_leftx, t_rightx, b_leftx, b_rightx;
	uint16_t t_lefty, t_righty, b_lefty, b_righty;
	bool tl, tr, bl, br;
	tl, tr, bl, br = false;
	

	t_leftx = tentativePos->x; t_lefty = tentativePos->y;	
	t_rightx = tentativePos->x+spriteinfo->w; t_righty = tentativePos->y;
	b_leftx = tentativePos->x; b_lefty = tentativePos->y+spriteinfo->h;	

	b_rightx = tentativePos->x+spriteinfo->w; 
	b_righty = tentativePos->y+spriteinfo->h;	

	if(walls.find(wallCoord(t_leftx/tile_w, t_lefty/tile_h)) 
			!= walls.end() && 
			walls.find(wallCoord(b_leftx/tile_w, b_lefty/tile_h)) 
				== walls.end()) {
		//printf("Y TOP left\n");
		return true;
	}
	if(walls.find(wallCoord(t_rightx/tile_w, t_righty/tile_h)) 
			!= walls.end() && 
			walls.find(wallCoord(b_rightx/tile_w, b_righty/tile_h)) 
				== walls.end())
	{
		//printf("Y TOP right\n");
		return true;
	}

	if(walls.find(wallCoord(t_leftx/tile_w, t_lefty/tile_h)) 
			== walls.end() && 
			walls.find(wallCoord(b_leftx/tile_w, b_lefty/tile_h)) 
				!= walls.end()) {
		//printf("Y BOTTOM left\n");
		return true;
	}

	if(walls.find(wallCoord(t_rightx/tile_w, t_righty/tile_h)) 
			== walls.end() && 
			walls.find(wallCoord(b_rightx/tile_w, b_righty/tile_h)) 
				!= walls.end())
	{
		//printf("Y BOTTOM right\n");
		return true;
	}
	return false;
}

bool
levelMap::collissionManager(myObject* object){
	// Check if the point is in collission
	SDL_Rect* tentativePos = object->getRect();
	SDL_Surface* spriteinfo = object->getSpriteSurface();
	uint16_t t_leftx, t_rightx, b_leftx, b_rightx;
	uint16_t t_lefty, t_righty, b_lefty, b_righty;
	bool tl, tr, bl, br;
	tl, tr, bl, br = false;

	t_leftx = tentativePos->x; t_lefty = tentativePos->y;	
	t_rightx = tentativePos->x+spriteinfo->w; t_righty = tentativePos->y;
	b_leftx = tentativePos->x; b_lefty = tentativePos->y+spriteinfo->h;	

	b_rightx = tentativePos->x+spriteinfo->w; 
	b_righty = tentativePos->y+spriteinfo->h;	
	if(walls.find(wallCoord(t_leftx/tile_w, t_lefty/tile_h)) 
			!= walls.end()){
		//printf("LEFT\n");
		return true;
	}
	if(walls.find(wallCoord(t_rightx/tile_w, t_righty/tile_h)) 
			!= walls.end()){
		//printf("RIGHT\n");
		return true;
	}
	if(walls.find(wallCoord(b_leftx/tile_w, b_lefty/tile_h)) 
			!= walls.end()){
		//printf("LEFT\n");
		return true;
	}
	if(walls.find(wallCoord(b_rightx/tile_w, b_righty/tile_h)) 
			!= walls.end()){
		//printf("RIGHT\n");
		return true;
	}
	// Calculate the grid ... 
	//Top Corner
	//printf("Top left X Location: %d \n", t_leftx/tile_w);
	//printf("Top left Y location: %d \n", t_lefty/tile_h);
	//printf("Top right X Location: %d \n", t_rightx/tile_w);
	//printf("Top right Y location: %d \n", t_righty/tile_h);
	//printf("Bottom left X Location: %d \n", b_leftx/tile_w);
	//printf("Bottom left Y location: %d \n", b_lefty/tile_h);
	//printf("Bottom right X Location: %d \n", b_rightx/tile_w);
	//printf("Bottom right Y location: %d \n", b_righty/tile_h);

	return false;

	//return true;
}

void
levelMap::drawMap(SDL_Renderer* _renderer){
	SDL_Rect temp;
	SDL_Rect renderQuad = {0,0,80,80};
	//std::cout << "Renderer : "<< _renderer << std::endl;
	//std::cout << map_rect.size() << std::endl;
	for( int i = 0; i < map_rect.size(); i++){
		temp.x = 80*(map_rect[i].first[0]-'0');
		temp.y = 80*(map_rect[i].first[1]-'0');
		temp.w = 80;
		temp.h = 80;
		//SDL_BlitSurface( tiles, &temp, screenSurface, map_rect[i].second );
		SDL_RenderCopy( _renderer, texture_tiles, &temp, map_rect[i].second);
	}
		//SDL_Rect renderQuad = {600, 700, 80, 90 };
		//SDL_RenderCopy( _renderer, texture_tiles, &renderQuad, &renderQuad);
		//SDL_Rect renderQuad2 = {0,80,80,80};
		//SDL_RenderCopy( _renderer, texture_tiles, &renderQuad2, &renderQuad2);
}

void
levelMap::loadMap(const char* mapLocation){
	gridmap.open("GRID.txt", std::ifstream::in);

	if( gridmap == NULL){
		printf("Failed to load gridmap\n");
	}
	else{
		printf("Loaded gridmap\n");

		char x[100], y[100], b;
		std::string dimension;
		std::getline(gridmap, dimension);
		sscanf(dimension.c_str(),"%s %s", x, y);
		dim_x = atoi(x); dim_y = atoi(y);
		printf("x: %d - y: %d\n", dim_x, dim_y);

		gridMap = new char**[dim_x];
		for(int i = 0; i < dim_x; i++){
			gridMap[i] = new char*[dim_y];
			for(int y = 0; y < dim_y; y++){
				gridMap[i][y] = new char[2];
			}
		}

		b = gridmap.get();
		short g_x = 0, g_y = 0;
		short x_y_counter = 0;
		while( gridmap.good() ){
			if( b == '\n' ){
				g_y++;
				g_x = 0;
				x_y_counter = 0;
			}
			else if( b == ' ' ){
				printf(" ");
				g_x++;
				x_y_counter = 0;
			}
			else{
				gridMap[g_x][g_y][x_y_counter] = b;
				x_y_counter++;
			}
			printf("%c",b);
			b = gridmap.get();
		}
		gridmap.close();

		printf("\n");

		walls.clear();
		for( int y = 0; y < dim_y; y++){
			for(int x = 0; x < dim_x; x++){
				std::cout << gridMap[x][y][0];
				std::cout << gridMap[x][y][1] << "  ";

				//Add a property check here for loading ...
				if( gridMap[x][y][0] == '0' &&  gridMap[x][y][1] == '0' ){
					std::string temp_name = std::to_string((long double)x) + 
						"," + std::to_string((long double)y);
					walls[temp_name] = 'W';

				}

				SDL_Rect* temp = new SDL_Rect();
				temp->x = 80*x;
				temp->y = 80*y;
				temp->w = 80;
				temp->h = 80;
				map_rect.push_back( std::make_pair(gridMap[x][y],temp) );

			}
			std::cout << std::endl;
		}

		printWalls();
	}
}


