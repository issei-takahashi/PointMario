#include "Font.h"

#include <SDL_ttf.h>

mario::Font::Font()
{
	// フォント初期化
	this->pFont = TTF_OpenFont("font/azuki.ttf", 24);
}

mario::Font::~Font()
{
	if( this->pFont ){
		TTF_CloseFont( this->pFont );
	}
}

int mario::Font::getPriority() const
{
	return 1;
}