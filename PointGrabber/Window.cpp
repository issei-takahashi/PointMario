#include "Window.h"

#include <SDL.h>
#include <SDL_image.h>

mario::Window::Window( int _width, int _height, string const & _windowName, bool _screenModeFlag )
	:width(_width),height(_height),windowName(_windowName),isScreenMode(_screenModeFlag)
{
	this->surface = SDL_SetVideoMode( _width, _height, 32, SDL_HWSURFACE );
	this->setScreenMode(_screenModeFlag);
	SDL_WM_SetCaption(_windowName.c_str(),NULL);
}

mario::Window::~Window()
{
	SDL_FreeSurface( this->surface );
}

void mario::Window::setScreenMode( bool _isScreenMode )
{
	if( this->isScreenMode != _isScreenMode ){
		this->isScreenMode = _isScreenMode;
		if( this->surface ){
			SDL_FreeSurface( this->surface );
		}
		if( this->isScreenMode ){
			this->surface = SDL_SetVideoMode( width, height, 32, SDL_HWSURFACE | SDL_FULLSCREEN );
		}
		else{
			this->surface = SDL_SetVideoMode( width, height, 32, SDL_HWSURFACE );
		}
	}
}

int mario::Window::getPriority() const
{
	return 1;
}

void mario::Window::wait( int _ms )
{
	SDL_Delay(_ms);
}