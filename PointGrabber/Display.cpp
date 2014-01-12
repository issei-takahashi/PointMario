#include "Display.h"
#include "Timer.h"
#include "Image.h"
#include "FileIO.h"
#include "WinRS.h"
#include "utils.h"

static std::string const SCREEN_CAPTION = "SDL window test";
bool mario::Display::isSDLinited = false;

mario::Display::Display( int _scrXmm, int _scrYmm, int _scrXpx, int _scrYpx )
	:isActive(false), pMainWindow(NULL), pFont(NULL), isFullScreen(false),
	screenXmm(_scrXmm), screenYmm(_scrYmm), screenXpx(_scrXpx), screenYpx(_scrYpx),
	crossSpeed(1.0)
{
	/* アクチュエータの初期化 */
	this->upActuator = ( unique_ptr<mario::Display::Actuator> )( new mario::Display::Actuator() );
}

mario::Display::~Display()
{
	this->stop();
}

void mario::Display::start()
{
	if( this->isActive == false ){
		this->isActive = true;
		if( Display::isSDLinited == false ){
			if( SDL_Init(SDL_INIT_EVERYTHING) == -1 ) {
				SDL_Quit();
			}
			if( TTF_Init() == -1 ) {
				TTF_Quit();
			}
			Display::isSDLinited = true;
		}
		static int const DISP_X_px = FileIO::getConst("DISP_X_px");
		static int const DISP_Y_px = FileIO::getConst("DISP_Y_px");
		this->pMainWindow = SDL_SetVideoMode( DISP_X_px, DISP_Y_px, 32, SDL_HWSURFACE );
		SDL_WM_SetCaption("Main Window",NULL);
		// フォント初期化
		this->pFont = TTF_OpenFont("font/azuki.ttf", 24); 
	}
}

void mario::Display::stop()
{
	if( this->isActive ){
		this->isActive = false;
		if( this->pMainWindow ){
			SDL_FreeSurface( this->pMainWindow );
		}
		if( this->pFont ){
			TTF_CloseFont( this->pFont );
		}
		if( Display::isSDLinited == true ){
			// finalize SDL
			SDL_Quit();
			TTF_Quit();
			Display::isSDLinited = false;
		}
	}
}

void mario::Display::oneLoop()
{
	this->keyInputEvent1();
	this->drawCross( this->crossPos, true );
}

void mario::Display::displayLoop()
{
	while( this->quitEvent() == false ){
		this->oneLoop();
	}
}

void mario::Display::drawCross( Coordinate<typeD> _pd, bool _printStringFLag )
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
			SDL_Rect rect, scr_rect;
			SDL_Color white = {0xff, 0xff, 0xff};
			string str = string("Pd = (") + utils::double2string( _pd.x ) +","+
				utils::double2string( _pd.y ) +","+
				utils::double2string( _pd.z ) + ")";
			SDL_Surface* pFontSurface = TTF_RenderUTF8_Blended( this->pFont, str.c_str(), white );
			rect.x = 0;
			rect.y = 0;
			rect.w = pFontSurface->w;
			rect.h = pFontSurface->h;

			/* 画像配置位置情報の設定 */
			int TEXT_SHIFT  = FileIO::getConst("TEXT_SHIFT");
			scr_rect.x = xpx+TEXT_SHIFT;
			scr_rect.y = ypx+TEXT_SHIFT;

			/* 描画 */
			/* サーフェスの複写 */
			SDL_BlitSurface( pFontSurface, &rect, SDL_GetVideoSurface(), &scr_rect );
		}
	}

	/* アクチュエータ(z) */
	{
		this->upActuator->moveTo( _pd.z );
	}

	/* サーフェスフリップ */
	SDL_Flip(SDL_GetVideoSurface());

}

void mario::Display::changeScreenMode()
{
	static int const DISP_X_px = FileIO::getConst("DISP_X_px");
	static int const DISP_Y_px = FileIO::getConst("DISP_Y_px");
	SDL_FreeSurface( this->pMainWindow );
	if( this->isFullScreen ){
		this->pMainWindow = SDL_SetVideoMode( DISP_X_px, DISP_Y_px, 32, SDL_HWSURFACE );
	}
	else{
		this->pMainWindow = SDL_SetVideoMode( DISP_X_px, DISP_Y_px, 32, SDL_HWSURFACE | SDL_FULLSCREEN );
	}
	this->isFullScreen = !this->isFullScreen;
}

bool mario::Display::quitEvent() const
{
	SDL_Event Qevnts;
	if( SDL_PollEvent( &Qevnts ) ){
		switch( Qevnts.type ){
		case SDL_QUIT:
			{
				return 1;
				break;
			}
		case SDL_KEYDOWN:
			{
				if( Qevnts.key.keysym.sym==SDLK_ESCAPE ||
					Qevnts.key.keysym.sym==SDLK_q ){
						return true;
				}
				break;
			}
		default: 
			{
				break;
			}
		}
	}
	return false;
}

bool mario::Display::keyInputEvent1()
{
	Uint8 *getKeys = SDL_GetKeyState(NULL);

	// 1～9キー
	times(i,0,9){
		if( getKeys[SDLK_1+i] == SDL_PRESSED ){
			this->crossSpeed = (double)(i+1);
			break;
		}
	}
	double speedBias = 1.0;
	// Shiftキー
	if( getKeys[SDLK_LSHIFT] == SDL_PRESSED ){
		speedBias = 0.1;
	}

	// 左右キー
	if       ( getKeys[SDLK_LEFT]  == SDL_PRESSED ){
		this->crossPos.x -= 1.0 * this->crossSpeed * speedBias;
	} else if( getKeys[SDLK_RIGHT] == SDL_PRESSED ){ 
		this->crossPos.x += 1.0 * this->crossSpeed * speedBias;
	}

	// 上下キー
	if       ( getKeys[SDLK_UP]    == SDL_PRESSED ){
		this->crossPos.y += 1.0 * this->crossSpeed * speedBias;
	} else if( getKeys[SDLK_DOWN]  == SDL_PRESSED ){ 
		this->crossPos.y -= 1.0 * this->crossSpeed * speedBias;
	}

	// 上下キー
	if       ( getKeys[SDLK_RSHIFT]    == SDL_PRESSED ){
		this->crossPos.z -= 1.0 * this->crossSpeed * speedBias;
	} else if( getKeys[SDLK_RCTRL]  == SDL_PRESSED ){ 
		this->crossPos.z += 1.0 * this->crossSpeed * speedBias;
	}	

	// Enterキー
	if( getKeys[SDLK_RETURN] ){
		return true;
	}

	return false;
}

void mario::Display::wait( int _ms )
{
	SDL_Delay(_ms);
}


/* ---------- アクチュエータ ---------- */

mario::Display::Actuator::Actuator()
	:zd(0.0)
{
	static int const ARDUINO_COM_NUM = FileIO::getConst("ARDUINO_COM_NUM");
	this->upPort = (unique_ptr<mario::WinRS>)( new mario::WinRS( ARDUINO_COM_NUM, 9600, ifLine::cr, "8N1", false ) );
}

mario::Display::Actuator::~Actuator()
{

}

void mario::Display::Actuator::moveTo( typeD _zd )
{
	this->upPort->putc1( max(0.0,_zd) );
}