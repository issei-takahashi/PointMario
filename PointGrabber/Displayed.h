#pragma once

namespace mario{

	class Displayed abstract
	{
	public:
		virtual int getPriority() const = 0;
		virtual void oneLoop() = 0;
	};

};