class textureWrapper{

	public:
		int mWidth, mHeight;
		textureWrapper();
		~textureWrapper();
		bool loadFromFile( std::string );
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		void free();
		bool loadFromFile(const char* path);
		SDL_Texture* mTexture;
	private:
};

textureWrapper::textureWrapper(){
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

bool
textureWrapper::loadFromFile(const char* path){
	free();

	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadSurface = IMG_Load( path );
	if( !loadSurface ){
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else{
		SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB( loadSurface->format,
														   0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface( renderer, loadSurface );
		if( !newTexture){
			printf("Unable to load image %s! SDL_image Error: %s\n", 
					path, IMG_GetError() );
		}
		else{
			mWidth = loadSurface->w;
			mHeight = loadSurface->h;
		}
		SDL_FreeSurface( loadSurface );
	}

	mTexture = newTexture;

	return mTexture != NULL;
}

void 
textureWrapper::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if(clip != NULL){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );
}

void
textureWrapper::free(){

	if( mTexture ){
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}


textureWrapper::~textureWrapper(){
	free();
}
