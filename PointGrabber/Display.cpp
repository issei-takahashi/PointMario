#include "Display.h"
#include "Timer.h"
#include "Image.h"

static std::string const SCREEN_CAPTION = "SDL window test";

mario::Display::Display()
{

}

mario::Display::~Display()
{

}

void mario::Display::start()
{
	if(SDL_Init(SDL_INIT_VIDEO)==-1) {
		SDL_Quit();
	}
	this->pMainWindow = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("Main Window",NULL);
}

void mario::Display::stop()
{
	// finalize SDL
	SDL_Quit();
}

void mario::Display::oneLoop()
{
	this->showImageTest();
}

void mario::Display::displayLoop()
{
	while( this->quitEvent() == false ){
		this->oneLoop();
	}
}

void PaintRect30( Uint16 x, Uint16 y, SDL_Surface* pSurface )
{
    SDL_Rect dstrect = { x, y, 30, 30 };
    
    SDL_FillRect( pSurface, &dstrect, 
        (rand()%2==0) ? ~rand() | 0x00050505 : rand() | 0x00202020);

}

void mario::Display::showImageTest()
{
	SDL_FillRect( this->pMainWindow,NULL,
	SDL_MapRGB(this->pMainWindow->format,255,0,0) );

	/* サーフェスの複写 */
	//SDL_BlitSurface(image, &rect, SDL_GetVideoSurface(), &scr_rect);
	static int x=0,y=0;
	::PaintRect30(x++,y++,this->pMainWindow);
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