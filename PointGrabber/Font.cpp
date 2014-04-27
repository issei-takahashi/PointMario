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

void mario::Font::oneLoop( uint _x, uint _y )
{
	SDL_Rect rect, scr_rect;
	SDL_Color white = {0xff, 0xff, 0xff};
	this->stringBuffer = "empty string";
	SDL_Surface* fontSurface = TTF_RenderUTF8_Blended( this->pFont, this->stringBuffer.c_str(), white );
	
	/* 画像の矩形情報設定 */
	rect.x = 0;
	rect.y = 0;
	rect.w = fontSurface->w;
	rect.h = fontSurface->h;

	/* 画像配置位置情報の設定 */
	scr_rect.x = _x;
	scr_rect.y = _y;

	/* 描画 */
	SDL_BlitSurface( fontSurface, &rect, SDL_GetVideoSurface(), &scr_rect );
}