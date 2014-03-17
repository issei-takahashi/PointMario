#pragma once

#include "Displayed.h"

struct SDL_Surface;

namespace mario
{
	class Image final : public Displayed
	{
		SHARED(Image);
	public:
		~Image();
		static shared_ptr<Image> makeShared( string const & _fileName );
		int getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
	private:
		Image( string const & _filePath );
		SDL_Surface* image;
		string const imageFilePath;
	};

};