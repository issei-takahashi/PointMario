#include "Window.h"

#include <SDL.h>
#include <SDL_image.h>
#include "SDL_macros.h"
#include "Displayed.h"

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

void mario::Window::addDisplayedElement( shared_ptr<mario::_Displayed> _ptr )
{
	_ptr->setWindow( this->shared_from_this() );
	auto itr = this->displayedElements.find(_ptr->getPriority());
	if( itr == this->displayedElements.end() ){
		multimap< Counter<_Displayed>, weak_ptr<_Displayed> > mm;
		mm.insert( make_pair( _ptr->getCount(), _ptr ) );
		this->displayedElements.insert( make_pair(_ptr->getPriority(),mm) );
	}else{
		this->displayedElements.insert(make_pair(_ptr->getPriority(),_ptr));
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
	for(auto it=this->displayedElements.begin();it!=this->displayedElements.end(); ){
		if(auto sp=it->second.lock()){
			auto d = sp->getDisplayPoint();
			sp->oneLoop(d.x,d.y);
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