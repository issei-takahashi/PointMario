#pragma once

#include "Coordinate.h"
#include "Displayed.h"

namespace issei
{
	class Cross : public mario::Displayed
	{
	public:
		virtual int getPriority() const { return 1; };
		virtual void oneLoop();
	};
};