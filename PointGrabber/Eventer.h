#pragma once

#include "KeyTypes.h"
#include "Singleton.h"

namespace mario
{
	class Eventer : public Singleton<Eventer>
	{
	public:
		bool quitEvent();
		bool keyEvent( mario::KeyType _key );
	private:
		Eventer(){}
	};
};