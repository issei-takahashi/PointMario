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
		void drawCross( Coordinate<typeD> _pd, bool _printStringFLag );
		void changeScreenMode();
		void setScreenMode( bool _fullScreenFlag );
		bool quitEvent() const;
		bool keyInputEvent1();
		void wait( int _ms );
		void set_crossPos( Coordinate<typeD> p ) { this->crossPos = p; }
		Coordinate<typeD> get_crossPos() const { return this->crossPos; }
	private:
		class Actuator
		{
		public:
			Actuator();
			~Actuator();
			void moveTo( typeD _zd );
		private:
			typeD zd;
			unique_ptr<class WinRS>  upPort; // Arduino�p�̃|�[�g
		};
	private:
		int const screenXmm;
		int const screenYmm;
		int const screenXpx;
		int const screenYpx;
	private:
		/* cross �֘A */
		Coordinate<typeD> crossPos;
		double crossSpeed;
		static bool isSDLinited;
		bool isActive;
		/* ���j�^�֘A */
		SDL_Surface *pMainWindow; //���C���E�B���h�E
		bool isFullScreen;        //�t���X�N���[�����[�h���ǂ���
		TTF_Font* pFont;           // �t�H���g
		/* �A�N�`���G�[�^�֘A */
		unique_ptr<mario::Display::Actuator> upActuator;
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