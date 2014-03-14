#pragma once

#include "Displayed.h"
#include "Shared.h"

struct SDL_Surface;

namespace mario
{
	class Surface final : public Displayed, public Shared<Surface>
	{
	public:
		Surface( string const & _fileName );
		~Surface();
		int getPriority() const final override;
		void oneLoop() final override;
	private:
		SDL_Surface* surface;
	};

};