#pragma once

#include "ColorRGB.h"

namespace mario
{
	class RGBColored
	{
	public:
		RGBColored(){}
		RGBColored( ColorRGB _c ):color(_c){}
		ColorRGB getColor() const { return this->color; }
		void setColor( ColorRGB const & _c ){ this->color = _c; }
		void setColor( uint r, uint g, uint b){ this->color = ColorRGB(r,g,b); }
		void setR(uint r){this->color.R=r;}
		void setG(uint g){this->color.G=g;}
		void setB(uint b){this->color.B=b;}
	protected:
		ColorRGB color;
	};
};