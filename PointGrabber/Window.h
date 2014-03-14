#pragma once

#include "Displayed.h"

struct SDL_Surface;

namespace mario{

	class Window final : public Displayed
	{
	public:
		Window( int _width, int _height, string const & _windowName, bool _screenModeFlag );
		~Window();
		int getPriority() const final override;
		void oneLoop() final override;
		void setScreenMode(bool _isScreenMode);
		void wait( int _ms );
	private:
		SDL_Surface* surface;
		int width;
		int height;
		string windowName;
		bool isScreenMode;
	};

};