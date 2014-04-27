#pragma once

#include "Displayed.h"

struct SDL_Surface;

namespace mario{

	class Window final : public enable_shared_from_this<Window>
	{
		SHARED(Window);
	public:
		Window( int _width, int _height, string const & _windowName, bool _screenModeFlag );
		~Window();
		void addDisplayedElement( shared_ptr<class _Displayed> _ptr );
		void removeDisplayedElement( shared_ptr<class _Displayed> _ptr );
		void oneLoop();
		void setScreenMode(bool _isScreenMode);
		void wait( int _ms );
		SDL_Surface* getSurface() const { return this->surface; }
	private:
		SDL_Surface* surface;
		int width;
		int height;
		string windowName;
		bool isScreenMode;
		map< priority, multimap< Counter<_Displayed> ,weak_ptr<class _Displayed> > > displayedElements;
	};

};