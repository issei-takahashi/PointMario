#pragma once

#include "Displayed.h"

namespace mario{

	class Cross final : public Displayed<Cross>
	{
		SHARED(Cross);
	public:
		Cross();
		~Cross();
		priority getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
	private:
	};

};