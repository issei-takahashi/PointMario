#include "Display.h"

static std::string const SCREEN_CAPTION = "SDL window test";

mario::Display::Display()
{
	bool isOK = true;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);

	if( isOK == false ){
		std::cerr << "ERROR: failed to initialize SDL" << std::endl;
		exit(1);
	}
}

mario::Display::~Display()
{
	// finalize SDL
	SDL_Quit();
}

void mario::Display::displayLoop()
{
	// main loop
	//while (true)
	{
		this->showImageTest();
	}
}

void mario::Display::showImageTest()
{
	SDL_Surface* image;
	SDL_Rect rect, scr_rect;

	/* �摜�ǂݍ��� */
	image = IMG_Load("image/sample.png");

	/* �摜�̋�`���ݒ� */
	rect.x = 0;
	rect.y = 0;
	rect.w = image->w;
	rect.h = image->h;

	/* �摜�z�u�ʒu���̐ݒ� */
	scr_rect.x = 0;
	scr_rect.y = 0;

	/* �T�[�t�F�X�̕��� */
	SDL_BlitSurface(image, &rect, SDL_GetVideoSurface(), &scr_rect);

	/* �T�[�t�F�X�t���b�v */
	SDL_Flip(SDL_GetVideoSurface());

	SDL_Delay(30000);

	SDL_FreeSurface(image);
}