#pragma once

namespace mario
{
	class Display
	{
	public:
		Display();
		~Display();
		static void displayThread();
		void mainLoopIncludingThreads();
		void displayLoop();
		void showImageTest();
		bool quitEvent() const;
		void keyInputEvent();
	private:
		void wait( int _ms );
	private:
		static int const SCREEN_WIDTH = 640;
		static int const SCREEN_HEIGHT = 640;
		static int const SCREEN_BPP = 32;
	private:
		 SDL_Surface *pMainWindow;//メインウィンドウ
	};

	class DisplayTimer
	{
	public:
		static Uint32 getTime(){
			return SDL_GetTicks();
		}
	private:
	};
};