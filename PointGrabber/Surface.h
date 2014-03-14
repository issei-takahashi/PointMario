#pragma once

#include "Displayed.h"

struct SDL_Surface;

namespace mario
{
	class Surface final : public Displayed
	{
		SHARED(Surface);
	public:
		int getPriority() const final override;
		void oneLoop() final override;
	private:
		SDL_Surface* surface;
		Surface( string const & _fileName );
		~Surface();
	};

};