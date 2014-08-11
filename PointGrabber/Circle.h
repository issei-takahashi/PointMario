#pragma once

#include "Displayed.h"
#include "ColorRGB.h"

namespace mario{

	class Circle final : public Displayed<Circle>
	{
		SHARED(Circle);
	public:
		static shared_ptr<Circle> makeShared( uint _radius );
		~Circle();
		priority getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
		ColorRGB color;
	private:
		Circle( uint _radius );
		uint radius;
	};

};