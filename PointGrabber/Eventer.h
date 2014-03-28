#pragma once

#include "KeyTypes.h"
#include "Singleton.h"

namespace mario
{
	class Eventer : public Singleton<Eventer>
	{
		friend class Singleton<Eventer>;
	public:
		bool quitEvent();
		void pollEvent();
		bool getIskeyPushed( mario::KeyType _key );
	private:
		Eventer(){}
	};
};