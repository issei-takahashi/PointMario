#include "Window.h"

#include <SDL.h>
#include <SDL_image.h>
#include "SDL_macros.h"

shared_ptr<mario::Window> mario::Window::makeShared( int _width, int _height, string const & _windowName, bool _screenModeFlag )
{
	return shared_ptr<Window>(new Window( _width, _height,_windowName,_screenModeFlag ));
}

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

void mario::Window::addDisplayedElement( shared_ptr<mario::Displayed> _ptr )
{
	_ptr->setWindow( this->shared_from_this() );
	this->displayedElements.insert(make_pair(_ptr->getPriority(),_ptr));
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
		}else{
			this->surface = SDL_SetVideoMode( width, height, 32, SDL_HWSURFACE );
		}
	}
}

void mario::Window::oneLoop()
{
	CLEAR_WINDOW(this->surface);
	for(auto it=this->displayedElements.begin();it!=this->displayedElements.end(); ){
		if(auto sp=it->second.lock()){
			sp->oneLoop(0,0);
			it++;
		}else{
			it = this->displayedElements.erase(it);
		}
	}
	FLIP_WINDOW;
}

void mario::Window::wait( int _ms )
{
	SDL_Delay(_ms);
}