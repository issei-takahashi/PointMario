#include "Circle.h"
#include <SDL.h>
#include <SDL_image.h>
#include "FileIO.h"
#include "Window.h"
#include "Display.h"
#include "SDL_utils.h"

using namespace mario;

shared_ptr<Circle> Circle::makeShared( uint _radius )
{
	return shared_ptr<Circle>(new Circle(_radius));
}

mario::Circle::Circle( uint _radius )
	:radius(_radius)
{

}

mario::Circle::~Circle()
{
}

int mario::Circle::getPriority() const
{
	return 1;
}

void mario::Circle::oneLoop( uint _x, uint _y )
{
	if( auto sp = this->ownerWindow.lock() ){
		auto windowSurface = sp->getSurface();
		/* ƒ‚ƒjƒ^ (x,y) */
		{
			int xpx = mario::Display::getInstance()->getPixX(_x);
			int ypx = mario::Display::getInstance()->getPixY(_y);
			SDL_DrawCircle(windowSurface,xpx-this->radius,ypx-this->radius,
				xpx+this->radius,ypx+this->radius,this->color.R,this->color.G,this->color.B);
		}
	}
}

