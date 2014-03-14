#pragma once

#include "Coordinate.h"
#include "Shared.h"

namespace mario
{
	class Display final : public Shared<Display>
	{
	public:
		Display( int _scrXmm, int _scrYmm, int _scrXpx, int _scrYpx );
		void oneLoop();
		void addDisplayedElement( shared_ptr<class Displayed> _ptr );
	private:
		multimap<int,weak_ptr<class Displayed> > displayedElements;
		int const screenXmm;
		int const screenYmm;
		int const screenXpx;
		int const screenYpx;
		/* アクチュエータ */
		class Actuator
		{
		public:
			Actuator();
			void moveTo( typeD _zd );
		private:
			typeD zd;
			unique_ptr<class WinRS>  upPort; // Arduino用のポート
		};
		/* アクチュエータ関連 */
		unique_ptr<mario::Display::Actuator> upActuator;
	};


	//class DisplayTimer
	//{
	//public:
	//	static Uint32 getTime(){
	//		return SDL_GetTicks();
	//	}
	//private:
	//};
};