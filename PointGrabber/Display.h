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
		void drawCross( Coordinate<typeD> _pd );
		bool quitEvent() const;
		void keyInputEvent();
		void wait( int _ms );
	private:
		int const screenXmm;
		int const screenYmm;
		int const screenXpx;
		int const screenYpx;
	private:
		/* モニタ関連 */
		SDL_Surface *pMainWindow; //メインウィンドウ
		bool isFullScreen;        //フルスクリーンモードかどうか
		/* アクチュエータ関連 */
		unique_ptr<class Actuator> upActuator;
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