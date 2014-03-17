#pragma once

namespace mario{

	class Displayed abstract
	{
	public:
		virtual int getPriority() const = 0;
		virtual void oneLoop( uint _x, uint _y ) = 0;
		void setWindow( shared_ptr<class Window> _ptr ){ this->ownerWindow = _ptr; }
	protected:
		weak_ptr<class Window> ownerWindow;
	};

};