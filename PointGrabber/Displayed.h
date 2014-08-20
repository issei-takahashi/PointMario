#pragma once

#include "Coordinate.h"
#include "Counter.h"
#include "Display.h"

namespace mario{
	typedef int priority;
	class _Displayed abstract : public Counter<_Displayed>
	{
	public:
		virtual ~_Displayed(){
		}
		virtual void setWindow( shared_ptr<class Window> _ptr ){ 
			this->_DisplayedMutex.lock();
			this->ownerWindow = _ptr; 
			this->_DisplayedMutex.unlock();
		}
		void setDisplayPoint( mario::Coordinate<mario::typeD> _d ){
			this->_DisplayedMutex.lock();
			this->displayPoint = _d; 
			this->_DisplayedMutex.unlock();
		}
		void addDisplayPoint( Eigen::Vector3d & _d ){
			this->_DisplayedMutex.lock();
			this->displayPoint += _d; 
			this->_DisplayedMutex.unlock();
		}
		mario::Coordinate<mario::typeD> getDisplayPoint() {
			this->_DisplayedMutex.lock();
			auto ret = this->displayPoint;
			this->_DisplayedMutex.unlock();
			return ret;
		}
		virtual priority getPriority() const = 0;
		virtual void oneLoop( uint _x, uint _y ) = 0;	
	protected:
		weak_ptr<class Window> ownerWindow;
	private:
		mario::Coordinate<mario::typeD> displayPoint;
		boost::mutex _DisplayedMutex;
	};

	template<class T>
	class Displayed abstract : public enable_shared_from_this<T>, public _Displayed
	{
	public:
		virtual ~Displayed(){
		}
		virtual void displayStart(){
			mario::Display::getInstance()->addDisplayedElement( this->shared_from_this() );
		}
		virtual void displayStop(){
			//mario::Display::getInstance()->removeDisplayedElement( this->shared_from_this() );
		}
	};

};