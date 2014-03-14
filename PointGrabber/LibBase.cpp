#include "LibBase.h"

#include <SDL.h>
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#include <SDL_image.h>
#pragma comment(lib, "SDL_image.lib")
#include <SDL_ttf.h>
#pragma comment(lib, "SDL_ttf.lib")

bool LibBase::initFlag = false;
bool LibBase::quitFlag = false;

void LibBase::initLib()
{
	assert( LibBase::initFlag==false );
	if( LibBase::initFlag == false ){
		if( SDL_Init(SDL_INIT_EVERYTHING) == -1 ) {
			SDL_Quit();
		}
		if( TTF_Init() == -1 ) {
			TTF_Quit();
		}
		LibBase::initFlag = true;
	}
}

void LibBase::quitLib()
{
	assert( LibBase::quitFlag==false );
	if( LibBase::quitFlag == false ){
		SDL_Quit();
		TTF_Quit();
		LibBase::quitFlag = true;
	}

}
