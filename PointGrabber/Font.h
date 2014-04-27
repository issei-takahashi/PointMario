#pragma once

#include "Displayed.h"

struct _TTF_Font;

namespace mario{

	class Font final : public Displayed<Font>
	{
		SHARED(Font);
	public:
		Font();
		~Font();
		priority getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
	private:
		_TTF_Font* pFont;
		string stringBuffer;
	};

};