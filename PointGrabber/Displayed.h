#pragma once

#include "Coordinate.h"
#include "Counter.h"

namespace mario{
	typedef int priority;
	class _Displayed abstract : public Counter<_Displayed>
	{
	public:
		virtual ~_Displayed(){
		}
		void setWindow( shared_ptr<class Window> _ptr ){ 
			this->ownerWindow = _ptr; 
		}
		void setDisplayPoint( mario::Coordinate<mario::typeD> _d ){
			this->displayPoint = _d; 
		}
		mario::Coordinate<mario::typeD> getDisplayPoint() const {
			return this->displayPoint; 
		}
		virtual priority getPriority() const = 0;
		virtual void oneLoop( uint _x, uint _y ) = 0;	
	protected:
		weak_ptr<class Window> ownerWindow;
	private:
		mario::Coordinate<mario::typeD> displayPoint;
	};

	template<class T>
	class Displayed abstract : public enable_shared_from_this<T>, public _Displayed
	{
	public:
		virtual ~Displayed(){
		}
		void displayStart(){
			mario::Display::getInstance()->addDisplayedElement( this->shared_from_this() );
		}
		void displayStop(){
			mario::Display::getInstance()->removeDisplayedElement( this->shared_from_this() );
		}
	};

};