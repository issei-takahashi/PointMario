#include "Window.h"

#include <SDL.h>
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#include <SDL_image.h>
#pragma comment(lib, "SDL_image.lib")
#include <SDL_ttf.h>
#pragma comment(lib, "SDL_ttf.lib")

#include "SDL_macros.h"
#include "Displayed.h"

mario::Window::Window( int _width, int _height, string const & _windowName, bool _screenModeFlag )
	:width(_width),height(_height),windowName(_windowName),isScreenMode(_screenModeFlag)
{
	if( SDL_Init(SDL_INIT_EVERYTHING) == -1 ) {
		SDL_Quit();
	}
	if( TTF_Init() == -1 ) {
		TTF_Quit();
	}
	this->surface = SDL_SetVideoMode( _width, _height, 32, SDL_HWSURFACE );
	this->setScreenMode(_screenModeFlag);
	SDL_WM_SetCaption(_windowName.c_str(),NULL);
}

mario::Window::~Window()
{
	// window‚ÉŒÀ‚Á‚Ä‚Í‚â‚Á‚¿‚áƒ_ƒ‚ç‚µ‚¢
	//SDL_FreeSurface( this->surface );
	SDL_Quit();
	TTF_Quit();
}

void mario::Window::addDisplayedElement( shared_ptr<mario::_Displayed> _ptr )
{
	_ptr->setWindow( this->shared_from_this() );
	auto itr = this->displayedElements.find(_ptr->getPriority());
	if( itr == this->displayedElements.end() ){
		multimap< Counter<_Displayed>, weak_ptr<_Displayed> > mm;
		mm.insert( make_pair( _ptr->getCount(), _ptr ) );
		this->displayedElements.insert( make_pair(_ptr->getPriority(),mm) );
	}else{
		itr->second.insert(make_pair(_ptr->getCount(),_ptr));
	}
}

void mario::Window::removeDisplayedElement( shared_ptr<mario::_Displayed> _ptr )
{
	_ptr->setWindow( NULL );
	auto itr = this->displayedElements.find(_ptr->getPriority());
	if( itr != this->displayedElements.end() ){
		auto range = itr->second.equal_range(_ptr->getCount());
		for(auto it = range.first; it != range.second; it++){
			if( it->second.lock() == _ptr ){
				itr->second.erase(it);
				break;
			}
		}
	}
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
	foreach(it,this->displayedElements){
		for( auto it2 = it->second.begin(); it2 != it->second.end(); ){
			if( auto sp = it2->second.lock() ){
				auto d = sp->getDisplayPoint();
				sp->oneLoop(d.x,d.y);
				it2++;
			}else{
				it2 = it->second.erase(it2);
			}
		}
	}
	FLIP_WINDOW;
}

void mario::Window::wait( int _ms )
{
	SDL_Delay(_ms);
}