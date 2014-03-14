#pragma once

#include "KeyTypes.h"

namespace mario
{
	class Eventer
	{
	public:
		bool quitEvent();
		bool keyEvent( mario::KeyType _key );
	};
};