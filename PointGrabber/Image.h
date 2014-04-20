#pragma once

#include "Displayed.h"

struct SDL_Surface;

#define IS_IMAGE_FILE(str) ((str).find(".png")!=string::npos||(str).find(".jpg")!=string::npos||(str).find(".bmp")!=string::npos)

namespace mario
{
	class Image final : public Displayed<Image>
	{
	public:
		SHARED(Image);
	public:
		~Image();
		static shared_ptr<Image> makeShared( string const & _fileName );
		priority getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
	private:
		Image( string const & _filePath );
		SDL_Surface* image;
		string const imageFilePath;
	};

};