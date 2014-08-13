#include "Cross.h"
#include <SDL.h>
#include <SDL_image.h>
#include "FileIO.h"
#include "Window.h"
#include "Display.h"

using namespace mario;

Cross::Cross()
	:rectShort(FileIO::getConst("CROSS_SHORT_px")),rectLong(FileIO::getConst("CROSS_LONG_px"))
{
	this->color.R = FileIO::getConst("CALIB_CROSS_R");
	this->color.G = FileIO::getConst("CALIB_CROSS_G");
	this->color.B = FileIO::getConst("CALIB_CROSS_B");
}

Cross::Cross(ColorRGB col)
	:rectShort(FileIO::getConst("CROSS_SHORT_px")),rectLong(FileIO::getConst("CROSS_LONG_px")),
	color(col)
{}

Cross::Cross(uint rShort,uint rLong)
	:rectShort(rShort),rectLong(rLong)
{}


Cross::Cross(ColorRGB col,uint rShort,uint rLong)
	:rectShort(rShort),rectLong(rLong),
	color(col)
{}

Cross::~Cross()
{}

int Cross::getPriority() const
{
	return 1;
}

void Cross::oneLoop( uint _x, uint _y )
{
	if( auto sp = this->ownerWindow.lock() ){
		auto windowSurface = sp->getSurface();
		SDL_FillRect( windowSurface,NULL, 
			SDL_MapRGB(windowSurface->format,0,0,0) );

		/* ƒ‚ƒjƒ^ (x,y) */
		{
			int col = SDL_MapRGB( windowSurface->format, this->color.R, this->color.G, this->color.B );

			int xpx = Display::getInstance()->getPixX(_x);
			int ypx = Display::getInstance()->getPixY(_y);

			SDL_Rect rect1 = { xpx - this->rectShort/2, ypx - this->rectLong/2 , this->rectShort, this->rectLong };
			SDL_Rect rect2 = { xpx - this->rectLong/2 , ypx - this->rectShort/2, this->rectLong , this->rectShort };
			SDL_FillRect( windowSurface, &rect1, col );
			SDL_FillRect( windowSurface, &rect2, col );
		}
	}
}

