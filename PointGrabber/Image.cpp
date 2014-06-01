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

		int xpx = mario::Display::getInstance()->getPixX(_x);
		int ypx = mario::Display::getInstance()->getPixY(_y);

		/* 画像の矩形情報設定 */
		SDL_Rect src_r = {0, 0, this->image->w, this->image->h};
		/* 画像配置位置情報の設定 */
		SDL_Rect dest_r = {xpx-this->image->w/2, ypx-this->image->h/2,xpx+this->image->w/2, ypx+this->image->h/2};

		/* サーフェスの複写 */
		auto windowSurface = sp->getSurface();
		SDL_BlitSurface( image, &src_r, windowSurface, &dest_r);
	}
}