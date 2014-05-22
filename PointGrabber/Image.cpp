#include "Image.h"
#include "Window.h"
#include "utils.h"
#include <SDL_image.h>
#pragma comment(lib, "SDL_image.lib")

mario::Image::Image( string const & _filePath )
	:imageFilePath(_filePath)
{

	string fullPath = utils::getExeDirectry() + _filePath;
	this->image = IMG_Load( fullPath.c_str() );
	if( this->image == NULL ){
		cout << SDL_GetError() << endl;
	}
}

mario::Image::~Image()
{
	SDL_FreeSurface( this->image );
}

int mario::Image::getPriority() const
{
	return 1;
}

void mario::Image::oneLoop( uint _x, uint _y )
{
	if( auto sp = this->ownerWindow.lock() ){
		SDL_Rect rect, scr_rect;

		int xpx = mario::Display::getInstance()->getPixX(_x);
		int ypx = mario::Display::getInstance()->getPixY(_y);

		/* 画像の矩形情報設定 */
		rect.x = 0;
		rect.y = 0;
		rect.w = this->image->w;
		rect.h = this->image->h;

		/* 画像配置位置情報の設定 */
		scr_rect.x = xpx;
		scr_rect.y = ypx;

		/* サーフェスの複写 */
		auto windowSurface = sp->getSurface();
		SDL_BlitSurface(image, &rect, windowSurface, &scr_rect);
	}
}