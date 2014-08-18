#pragma once

#include "Displayed.h"
#include "RGBColored.h"
#include "ColorRGB.h"

namespace mario{

	class Circle final : public Displayed<Circle>, public RGBColored
	{
		SHARED(Circle);
	public:
		static shared_ptr<Circle> makeShared( uint _radius, ColorRGB _col );
		~Circle();
		priority getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
	private:
		Circle( uint _radius, ColorRGB _col );
		uint radius;
	};

};