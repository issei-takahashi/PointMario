#pragma once

#include "Coordinate.h"
#include "Singleton.h"

namespace mario
{
	class Display final : public Singleton<Display>
	{
	public:
		void oneLoop();
	private:
		Display();
		shared_ptr<class Window> window;
		int screenXmm;
		int screenYmm;
		int screenXpx;
		int screenYpx;
		/* アクチュエータ */
		class Actuator
		{
			SHARED(Actuator);
		public:
			shared_ptr<Actuator> makeShared();
			void moveTo( typeD _zd );
		private:
			Actuator();
			~Actuator();
			typeD zd;
			unique_ptr<class WinRS>  arduinoPort; // Arduino用のポート
		};
		/* アクチュエータ関連 */
		shared_ptr<mario::Display::Actuator> actuator;
	};
};