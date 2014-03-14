#pragma once

#include "Coordinate.h"
#include "Singleton.h"

namespace mario
{
	class Display final : public Singleton<Display>
	{
	public:
		void oneLoop();
		void addDisplayedElement( shared_ptr<class Displayed> _ptr );
	private:
		Display();
		unique_ptr<class Window> window;
		multimap<int,weak_ptr<class Displayed> > displayedElements;
		int screenXmm;
		int screenYmm;
		int screenXpx;
		int screenYpx;
		/* �A�N�`���G�[�^ */
		class Actuator
		{
		public:
			Actuator();
			void moveTo( typeD _zd );
		private:
			typeD zd;
			unique_ptr<class WinRS>  upPort; // Arduino�p�̃|�[�g
		};
		/* �A�N�`���G�[�^�֘A */
		unique_ptr<mario::Display::Actuator> upActuator;
	};
};