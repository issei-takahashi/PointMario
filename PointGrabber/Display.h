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
		Display();
		void oneLoop();
		void moveActuatorTo( typeD _z );
		void addDisplayedElement( shared_ptr<class _Displayed> _ptr );
		void setScreenMode( bool _isScreenMode );
		void wait( int _ms );
		void closeWindow();
		pix getPixX( typeD _dx ) const;
		pix getPixY( typeD _dy ) const;
		int getScreenXmm() const { return this->screenXmm; }
		int getScreenYmm() const { return this->screenYmm; }
		int getScreenXpx() const { return this->screenXpx; }
		int getScreenYpx() const { return this->screenYpx; }
	private:
		shared_ptr<class Window> window;
		int screenXmm;
		int screenYmm;
		int screenXpx;
		int screenYpx;
		/* アクチュエータ */
		class Actuator
		{
		public:
			Actuator();
			~Actuator();
			void moveTo( typeD _zd );
		private:
			typeD zd;
			unique_ptr<class WinRS>  upPort; // Arduino用のポート
		};
		/* アクチュエータ関連 */
		shared_ptr<mario::Display::Actuator> actuator;
	};
};