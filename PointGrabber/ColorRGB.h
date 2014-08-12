#pragma once

namespace mario
{
	class ColorRGB
	{
	public:
		ColorRGB()
			:R(255),G(255),B(255){}
		ColorRGB(uint _r, uint _g, uint _b)
			:R(_r),G(_g),B(_b){}
		uint R;
		uint G;
		uint B;
		uint getH() const;
		uint getS() const;
		uint getV() const;
	private:

	};
};