#pragma once

#include "Coordinate.h"

namespace mario
{
	class Display
	{
	public:
		Display( int _scrXmm, int _scrYmm, int _scrXpx, int _scrYpx );
		~Display();
		void start();
		void stop();
		void oneLoop();
		void displayLoop();
		void showImageTest();
		bool quitEvent() const;
		void keyInputEvent();
		void wait( int _ms );
	private:
		int const screenXmm;
		int const screenYmm;
		int const screenXpx;
		int const screenYpx;
	private:
		 SDL_Surface *pMainWindow; //メインウィンドウ
		 bool isFullScreen;        //フルスクリーンモード
		 Coordinate<typeD> calibMarkerPos; // キャリブレーションマーカーの位置
	};

	class Actuator
	{
	public:
		Actuator();
		~Actuator();
		void moveTo( typeD _zd );
	private:
		typeD zd;
		unique_ptr<class WinRS>  upPort; // Arduino用のポート
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