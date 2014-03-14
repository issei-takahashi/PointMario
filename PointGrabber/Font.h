#pragma once

#include "Displayed.h"
#include "Shared.h"

struct _TTF_Font;

namespace mario{

	class Font final : public Displayed, public Shared<Font>
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