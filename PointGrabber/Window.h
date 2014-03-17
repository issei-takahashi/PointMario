#pragma once

#include "Displayed.h"

struct SDL_Surface;

namespace mario{

	class Window final : public enable_shared_from_this<Window>
	{
		SHARED(Window);
	public:
		~Window();
		static shared_ptr<Window> makeShared( int _width, int _height, string const & _windowName, bool _screenModeFlag );
		void addDisplayedElement( shared_ptr<class Displayed> _ptr );
		void oneLoop();
		void setScreenMode(bool _isScreenMode);
		void wait( int _ms );
		SDL_Surface* getSurface() const { return this->surface; }
	private:
		Window( int _width, int _height, string const & _windowName, bool _screenModeFlag );
		SDL_Surface* surface;
		int width;
		int height;
		string windowName;
		bool isScreenMode;
		multimap<int,weak_ptr<class Displayed> > displayedElements;
	};

};