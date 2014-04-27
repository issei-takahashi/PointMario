#include "Display.h"
#include "Timer.h"
#include "Image.h"
#include "FileIO.h"
#include "WinRS.h"
#include "utils.h"
#include "Displayed.h"
#include "Window.h"

using namespace mario;

#define MAKE_WINDOW_IF_NOT_EXIST if(this->window==NULL)this->window=(shared_ptr<Window>)(new Window(this->screenXpx,this->screenYpx,"Main Window",false))


static std::string const SCREEN_CAPTION = "SDL window test";

Display::Display()
{
	this->screenXmm = FileIO::getConst("DISP_X_mm");
	this->screenYmm = FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = FileIO::getConst("DISP_Z_mm");
	this->screenXpx = FileIO::getConst("DISP_X_px");
	this->screenYpx = FileIO::getConst("DISP_Y_px");

	/* アクチュエータの初期化 */
	this->actuator = (shared_ptr<Actuator>)(new Actuator());
	MAKE_WINDOW_IF_NOT_EXIST;
}

void Display::oneLoop()
{
	MAKE_WINDOW_IF_NOT_EXIST;
	this->window->oneLoop();
}

void Display::moveActuatorTo( typeD _z )
{
	if(this->actuator){
		this->actuator->moveTo(_z);
	}
}

void Display::addDisplayedElement( shared_ptr<_Displayed> _ptr )
{
	MAKE_WINDOW_IF_NOT_EXIST;
	this->window->addDisplayedElement(_ptr);
}

void Display::setScreenMode( bool _isScreenMode )
{
	MAKE_WINDOW_IF_NOT_EXIST;
	this->window->setScreenMode(_isScreenMode);
}

void Display::wait( int _ms )
{
	MAKE_WINDOW_IF_NOT_EXIST;
	this->window->wait(_ms);
}

void Display::closeWindow()
{
	if(this->window){
		this->window.reset();
	}
}

pix Display::getPixX( typeD _dx ) const
{
	return _dx * this->screenXpx / this->screenXmm;
}

pix Display::getPixY( typeD _dy ) const
{
	return _dy * this->screenYpx / this->screenYmm;
}



/* ---------- アクチュエータ ---------- */

Display::Actuator::Actuator()
	:zd(0.0)
{
	static int const ARDUINO_COM_NUM = FileIO::getConst("ARDUINO_COM_NUM");
	this->upPort = (unique_ptr<WinRS>)( new WinRS( ARDUINO_COM_NUM, 9600, ifLine::cr, "8N1", false ) );
}

Display::Actuator::~Actuator()
{

}

void Display::Actuator::moveTo( typeD _zd )
{
	this->upPort->putc1( max(0.0,_zd) );
}