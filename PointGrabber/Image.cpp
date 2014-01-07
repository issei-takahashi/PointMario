#include "Image.h"

mario::Image::Image( string const & _fileName )
{
	this->pSurface = IMG_Load( _fileName.c_str() );
}

mario::Image::~Image()
{
	SDL_FreeSurface( this->pSurface );
}