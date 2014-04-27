#include "Display.h"
#include "Timer.h"
#include "Image.h"
#include "FileIO.h"
#include "WinRS.h"
#include "utils.h"
#include "Displayed.h"
#include "Window.h"

#define MAKE_WINDOW_IF_NOT_EXIST if(this->window==NULL)this->window=mario::Window::makeShared(this->screenXpx,this->screenYpx,"Main Window",false)


static std::string const SCREEN_CAPTION = "SDL window test";

mario::Display::Display()
{
	this->screenXmm = mario::FileIO::getConst("DISP_X_mm");
	this->screenYmm = mario::FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = mario::FileIO::getConst("DISP_Z_mm");
	this->screenXpx = mario::FileIO::getConst("DISP_X_px");
	this->screenYpx = mario::FileIO::getConst("DISP_Y_px");

	/* アクチュエータの初期化 */
	this->actuator = (shared_ptr<Actuator>)(new Actuator());
	MAKE_WINDOW_IF_NOT_EXIST;
}

void mario::Display::oneLoop()
{
	MAKE_WINDOW_IF_NOT_EXIST;
	this->window->oneLoop();
}

void mario::Display::moveActuatorTo( typeD _z )
{
	if(this->actuator){
		this->actuator->moveTo(_z);
	}
}

void mario::Display::addDisplayedElement( shared_ptr<_Displayed> _ptr )
{
	MAKE_WINDOW_IF_NOT_EXIST;
	this->window->addDisplayedElement(_ptr);
}

void mario::Display::setScreenMode( bool _isScreenMode )
{
	MAKE_WINDOW_IF_NOT_EXIST;
	this->window->setScreenMode(_isScreenMode);
}

void mario::Display::wait( int _ms )
{
	MAKE_WINDOW_IF_NOT_EXIST;
	this->window->wait(_ms);
}

void mario::Display::closeWindow()
{
	if(this->window){
		this->window.reset();
	}
}

pix mario::Display::getPixX( typeD _dx ) const
{
	return _dx * this->screenXpx / this->screenXmm;
}

pix mario::Display::getPixY( typeD _dy ) const
{
	return _dy * this->screenYpx / this->screenYmm;
}



/* ---------- アクチュエータ ---------- */

mario::Display::Actuator::Actuator()
	:zd(0.0)
{
	static int const ARDUINO_COM_NUM = FileIO::getConst("ARDUINO_COM_NUM");
	this->upPort = (unique_ptr<mario::WinRS>)( new mario::WinRS( ARDUINO_COM_NUM, 9600, ifLine::cr, "8N1", false ) );
}

mario::Display::Actuator::~Actuator()
{

}

void mario::Display::Actuator::moveTo( typeD _zd )
{
	this->upPort->putc1( max(0.0,_zd) );
}