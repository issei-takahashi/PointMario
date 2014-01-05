#include "Display.h"

static std::string const SCREEN_CAPTION = "SDL window test";

mario::Display::Display()
{
	bool isOK = true;
	// initialize SDL
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		isOK =  false;
	}
	// create indow
	this->pWindow = SDL_CreateWindow( 
		SCREEN_CAPTION.c_str(),
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		640, 480, 0);
	if (!pWindow){
		isOK = false;
	}

	if( isOK == false ){
		std::cerr << "ERROR: failed to initialize SDL" << std::endl;
		exit(1);
	}
}

mario::Display::~Display()
{
	// finalize SDL
	SDL_Quit();
}

void mario::Display::displayLoop()
{
	// main loop
	while (true)
	{
		if ( !pollingEvent() ){
			break;
		}
	}
}

bool mario::Display::pollingEvent()
{
	SDL_Event ev;
	SDL_Keycode key;
	while ( SDL_PollEvent(&ev) )
	{
		switch(ev.type){
		case SDL_QUIT:
			// raise when exit event is occur
			return false;
			break;
		case SDL_KEYDOWN:
			// raise when key down
			{
				key=ev.key.keysym.sym;
				// ESC
				if(key==27){
					return false;
				}
			}
			break;
		}
	}
	return true;
}