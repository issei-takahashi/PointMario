#include "Display.h"
#include "Timer.h"
#include "Image.h"

static std::string const SCREEN_CAPTION = "SDL window test";

mario::Display::Display()
{
	if(SDL_Init(SDL_INIT_VIDEO)==-1) {
		SDL_Quit();
	}
	this->pMainWindow = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("Main Window",NULL);
}

mario::Display::~Display()
{
	// finalize SDL
	SDL_Quit();
}

static mario::Display g_disp;
void mario::Display::displayThread()
{
	g_disp.displayLoop();
}

#include "MeasureBasement.h"
void mario::Display::mainLoopIncludingThreads()
{
	mario::MeasureBasement base;

	boost::thread th1( boost::bind( &mario::MeasureBasement::measureLoop, &base ) );
	//boost::thread th2( boost::bind( &mario::Display::displayLoop, &disp ) );

	this->displayLoop();
	th1.join();
	//th2.join();
}

void mario::Display::displayLoop()
{
	while( this->quitEvent() == false ){
		auto ms1 = mario::DisplayTimer::getTime();
		this->showImageTest();
		auto ms2 = mario::DisplayTimer::getTime();
		if( ms2 - ms1 < 16 ){
			this->wait( 16 - ( ms2 - ms1 ) );
		}
	}
}

void mario::Display::showImageTest()
{
	SDL_FillRect( this->pMainWindow,NULL,
		SDL_MapRGB(this->pMainWindow->format,255,0,0) );
	SDL_Rect rect, scr_rect;

	/* 画像の矩形情報設定 */
	rect.x = 0;
	rect.y = 0;
	//rect.w = image->w;
	//rect.h = image->h;

	/* 画像配置位置情報の設定 */
	scr_rect.x = 0;
	scr_rect.y = 0;

	scr_rect.x += 1;
	scr_rect.y += 1;
	/* サーフェスの複写 */
	//SDL_BlitSurface(image, &rect, SDL_GetVideoSurface(), &scr_rect);

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

	//-----------------------------------Ｚキー確認

	if( getKeys[SDLK_z]==SDL_PRESSED )
	{
		;
	}

}

void mario::Display::wait( int _ms )
{
	SDL_Delay(_ms);
}