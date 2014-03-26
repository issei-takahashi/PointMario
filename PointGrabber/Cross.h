#pragma once

#include "Displayed.h"

namespace mario{

	class Cross final : public Displayed
	{
		SHARED(Cross);
	public:
		static shared_ptr<Cross> makeShared();
		~Cross();
		priority getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
	private:
		Cross();
	};

};