#include "Image.h"
#include "Window.h"
#include <SDL_image.h>

mario::Image::Image( string const & _filePath )
	:imageFilePath(_filePath)
{
	this->image = IMG_Load( _filePath.c_str() );
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

		/* �摜�̋�`���ݒ� */
		rect.x = 0;
		rect.y = 0;
		rect.w = this->image->w;
		rect.h = this->image->h;

		/* �摜�z�u�ʒu���̐ݒ� */
		scr_rect.x = _x;
		scr_rect.y = _y;

		/* �T�[�t�F�X�̕��� */
		auto windowSurface = sp->getSurface();
		SDL_BlitSurface(image, &rect, windowSurface, &scr_rect);
	}
}