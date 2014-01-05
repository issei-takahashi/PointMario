#pragma once

namespace mario
{
	class Display
	{
	public:
		Display();
		~Display();
		void displayLoop();
		void showImageTest();
	private:
		static int const SCREEN_WIDTH = 640;
		static int const SCREEN_HEIGHT = 640;
		static int const SCREEN_BPP = 32;
	};
};