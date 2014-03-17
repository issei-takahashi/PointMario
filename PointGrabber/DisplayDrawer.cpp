#include "DisplayDrawer.h"

void issei::Cross::oneLoop(){
{
	SDL_FillRect( this->pMainWindow,NULL, 
		SDL_MapRGB(this->pMainWindow->format,0,0,0) );

	/* モニタ (x,y) */
	{
		static int const cR = FileIO::getConst("CALIB_CROSS_R");
		static int const cG = FileIO::getConst("CALIB_CROSS_G");
		static int const cB = FileIO::getConst("CALIB_CROSS_B");
		int red = SDL_MapRGB( this->pMainWindow->format, cR, cG, cB ); 
		int xpx = _pd.x * ( (double)this->screenXpx / this->screenXmm );
		static int const DISP_Y_mm = FileIO::getConst("DISP_Y_mm");
		int ypx = (double)( DISP_Y_mm - _pd.y ) * ( (double)this->screenYpx / this->screenYmm );
		int CROSS_SHORT = FileIO::getConst("CROSS_SHORT_px");
		int CROSS_LONG  = FileIO::getConst("CROSS_LONG_px");
		SDL_Rect rect1 = { xpx - CROSS_SHORT/2, ypx - CROSS_LONG/2 , CROSS_SHORT, CROSS_LONG };
		SDL_Rect rect2 = { xpx - CROSS_LONG/2 , ypx - CROSS_SHORT/2, CROSS_LONG , CROSS_SHORT };
		SDL_FillRect( this->pMainWindow, &rect1, red );
		SDL_FillRect( this->pMainWindow, &rect2, red );
		if( _printStringFLag ){
			//SDL_Rect rect, scr_rect;
			//SDL_Color white = {0xff, 0xff, 0xff};
			//string str = string("Pd = (") + utils::double2string( _pd.x ) +","+
			//	utils::double2string( _pd.y ) +","+
			//	utils::double2string( _pd.z ) + ")";
			//SDL_Surface* pFontSurface = TTF_RenderUTF8_Blended( this->pFont, str.c_str(), white );
			//rect.x = 0;
			//rect.y = 0;
			//rect.w = pFontSurface->w;
			//rect.h = pFontSurface->h;

			///* 画像配置位置情報の設定 */
			//int TEXT_SHIFT  = FileIO::getConst("TEXT_SHIFT");
			//scr_rect.x = xpx+TEXT_SHIFT;
			//scr_rect.y = ypx+TEXT_SHIFT;

			///* 描画 */
			///* サーフェスの複写 */
			//SDL_BlitSurface( pFontSurface, &rect, SDL_GetVideoSurface(), &scr_rect );
		}
	}

	/* アクチュエータ(z) */
	{
		this->upActuator->moveTo( 300-_pd.z );
	}

	/* サーフェスフリップ */
	SDL_Flip(SDL_GetVideoSurface());

}
