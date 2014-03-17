#pragma once

#include "Displayed.h"

struct _TTF_Font;

namespace mario{

	class Font final : public Displayed
	{
		SHARED(Font);
	public:
		static shared_ptr<Font> makeShared();
		~Font();
		int getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
	private:
		Font();
		_TTF_Font* pFont;
		string stringBuffer;
	};

};