#pragma once

namespace mario
{
	class Image
	{
	public:
		Image( string const & _fileName );
		~Image();
	private:
		SDL_Surface* pSurface;
	};
};