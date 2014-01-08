#include "Display.h"
#include "Timer.h"
#include "Image.h"
#include "FileIO.h"
#include "WinRS.h"

static std::string const SCREEN_CAPTION = "SDL window test";

mario::Display::Display( int _scrXmm, int _scrYmm, int _scrXpx, int _scrYpx )
	:pMainWindow(NULL), isFullScreen(false),
	screenXmm(_scrXmm), screenYmm(_scrYmm), screenXpx(_scrXpx), screenYpx(_scrYpx)
{
	/* アクチュエータの初期化 */
	this->upActuator = ( unique_ptr<mario::Actuator> )( new Actuator() );
}

mario::Display::~Display()
{

}

void mario::Display::start()
{
	if( SDL_Init(SDL_INIT_EVERYTHING) == -1 ) {
		SDL_Quit();
	}
	static int const DISP_X_px = FileIO::getConst("DISP_X_px");
	static int const DISP_Y_px = FileIO::getConst("DISP_Y_px");
	this->pMainWindow = SDL_SetVideoMode( DISP_X_px, DISP_Y_px, 32, SDL_HWSURFACE );
	SDL_WM_SetCaption("Main Window",NULL);
}

void mario::Display::stop()
{
	// finalize SDL
	SDL_Quit();
}

void mario::Display::oneLoop()
{
	this->keyInputEvent();
	static int count = 0;
	Coordinate<typeD> p(count,count,count);
	this->drawCross( p );
	count++;
}

void mario::Display::displayLoop()
{
	while( this->quitEvent() == false ){
		this->oneLoop();
	}
}

void mario::Display::drawCross( Coordinate<typeD> _pd )
{
	SDL_FillRect( this->pMainWindow,NULL, 
		SDL_MapRGB(this->pMainWindow->format,0,0,0) );

	/* モニタ (x,y) */
	{
		static int const cR = FileIO::getConst("CALIB_CROSS_R");
		static int const cG = FileIO::getConst("CALIB_CROSS_G");
		static int const cB = FileIO::getConst("CALIB_CROSS_B");
		int red = SDL_MapRGB( this->pMainWindow->format, cR, cG, cB ); 
		int xpx = _pd.x * ( this->screenXpx / this->screenXmm );
		static int const DISP_Y_mm = FileIO::getConst("DISP_Y_mm");
		int ypx = (double)( DISP_Y_mm - _pd.y ) * ( (double)this->screenYpx / this->screenYmm );
		int CROSS_SHORT = FileIO::getConst("CROSS_SHORT_px");
		int CROSS_LONG  = FileIO::getConst("CROSS_LONG_px");
		SDL_Rect rect1 = { xpx - CROSS_SHORT/2, ypx - CROSS_LONG/2 , CROSS_SHORT, CROSS_LONG };
		SDL_Rect rect2 = { xpx - CROSS_LONG/2 , ypx - CROSS_SHORT/2, CROSS_LONG , CROSS_SHORT };
		SDL_FillRect( this->pMainWindow, &rect1, red );
		SDL_FillRect( this->pMainWindow, &rect2, red );
	}

	/* アクチュエータ(z) */
	{
		this->upActuator->moveTo( _pd.z );
	}

	/* サーフェスフリップ */
	SDL_Flip(SDL_GetVideoSurface());

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

void mario::Display::keyInputEvent()
{
	Uint8 *getKeys = SDL_GetKeyState(NULL);
	//-----------------------------------矢印キー確認
	if( getKeys[SDLK_LEFT] == SDL_PRESSED )
		;
	else 
		;
	if( getKeys[SDLK_RIGHT] == SDL_PRESSED )
		; 
	else 
		0;

	//-----------------------------------Wキー確認

	if( getKeys[SDLK_w]==SDL_PRESSED )
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

}

void mario::Display::wait( int _ms )
{
	SDL_Delay(_ms);
}


/* ---------- アクチュエータ ---------- */

mario::Actuator::Actuator()
	:zd(0.0)
{
	static int const ARDUINO_COM_NUM = FileIO::getConst("ARDUINO_COM_NUM");
	this->upPort = (unique_ptr<mario::WinRS>)( new mario::WinRS( ARDUINO_COM_NUM, 9600, ifLine::cr, "8N1", false ) );
}

mario::Actuator::~Actuator()
{

}

void mario::Actuator::moveTo( typeD _zd )
{
	this->upPort->putc1( max(0.0,_zd) );
}