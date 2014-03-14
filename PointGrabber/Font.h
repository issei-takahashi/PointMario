#pragma once

#include "Displayed.h"

struct _TTF_Font;

namespace mario{

	class Font final : public Displayed
	{
	public:
		Font();
		~Font();
		int getPriority() const final override;
		void oneLoop() final override;
	private:
		_TTF_Font* pFont;
	};

};