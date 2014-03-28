#include "Display.h"
#include "Timer.h"
#include "Image.h"
#include "FileIO.h"
#include "WinRS.h"
#include "utils.h"
#include "Displayed.h"
#include "Window.h"

static std::string const SCREEN_CAPTION = "SDL window test";

mario::Display::Display()
{
	this->screenXmm = mario::FileIO::getConst("DISP_X_mm");
	this->screenYmm = mario::FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = mario::FileIO::getConst("DISP_Z_mm");
	this->screenXpx = mario::FileIO::getConst("DISP_X_px");
	this->screenYpx = mario::FileIO::getConst("DISP_Y_px");

	/* アクチュエータの初期化 */
	this->actuator = ( unique_ptr<mario::Display::Actuator> )( new mario::Display::Actuator() );
	/* ウィンドウ初期化 */
	this->window = mario::Window::makeShared(this->screenXpx,this->screenYpx,"Main Window",false);
}

void mario::Display::oneLoop()
{
	if(this->window){
		this->window->oneLoop();
	}
}

void mario::Display::moveActuatorTo( typeD _z )
{
	if(this->actuator){
		this->actuator->moveTo(_z);
	}
}

void mario::Display::addDisplayedElement( shared_ptr<_Displayed> _ptr )
{
	if(this->window){
		this->window->addDisplayedElement(_ptr);
	}
}

void mario::Display::setScreenMode( bool _isScreenMode )
{

}

void mario::Display::wait( int _ms )
{

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

shared_ptr<mario::Display::Actuator> mario::Display::Actuator::makeShared()
{
	return (shared_ptr<mario::Display::Actuator>)(new Actuator());
}

mario::Display::Actuator::Actuator()
	:zd(0.0)
{
	static int const ARDUINO_COM_NUM = FileIO::getConst("ARDUINO_COM_NUM");
	this->arduinoPort = (unique_ptr<mario::WinRS>)( new mario::WinRS( ARDUINO_COM_NUM, 9600, ifLine::cr, "8N1", false ) );
}

void mario::Display::Actuator::moveTo( typeD _zd )
{
	this->arduinoPort->putc1( max(0.0,_zd) );
}