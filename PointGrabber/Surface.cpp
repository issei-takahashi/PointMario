#include "Surface.h"

#include <SDL_image.h>

mario::Surface::Surface( string const & _fileName )
{
	this->surface = IMG_Load( _fileName.c_str() );
}

mario::Surface::~Surface()
{
	SDL_FreeSurface( this->surface );
}

int mario::Surface::getPriority() const
{
	return 1;
}