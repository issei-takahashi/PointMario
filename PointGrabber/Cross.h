#pragma once

#include "Displayed.h"
#include "ColorRGB.h"

namespace mario{

	class Cross final : public Displayed<Cross>
	{
		SHARED(Cross);
	public:
		Cross();
		Cross(ColorRGB col);
		Cross(uint rShort,uint rLong);
		Cross(ColorRGB col,uint rShort,uint rLong);
		~Cross();
		priority getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
	private:
		ColorRGB color;
		uint const rectShort;
		uint const rectLong;
	};

};