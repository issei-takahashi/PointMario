#pragma once

#include "Coordinate.h"
#include "Singleton.h"

typedef int pix;

namespace mario
{
	class Display final : public Singleton<Display>
	{
		friend class Singleton<Display>;
	public:
		void oneLoop();
		void moveActuatorTo( typeD _z );
		void addDisplayedElement( shared_ptr<class _Displayed> _ptr );
		void setScreenMode( bool _isScreenMode );
		void wait( int _ms );
		void closeWindow();
		pix getPixX( typeD _dx ) const;
		pix getPixY( typeD _dy ) const;
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
			static shared_ptr<Actuator> makeShared();
			void moveTo( typeD _zd );
		private:
			Actuator();
			typeD zd;
			uint portNum;
			shared_ptr<class WinRS>  arduinoPort; // Arduino用のポート
		};
		/* アクチュエータ関連 */
		shared_ptr<mario::Display::Actuator> actuator;
	};
};