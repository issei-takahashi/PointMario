#include "Display.h"
#include "Timer.h"
#include "Surface.h"
#include "FileIO.h"
#include "WinRS.h"
#include "utils.h"
#include "Displayed.h"
#include "Window.h"
#include "Surface.h"

static std::string const SCREEN_CAPTION = "SDL window test";

mario::Display::Display()
{
	this->screenXmm = mario::FileIO::getConst("DISP_X_mm");
	this->screenYmm = mario::FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = mario::FileIO::getConst("DISP_Z_mm");
	this->screenXpx = mario::FileIO::getConst("DISP_X_px");
	this->screenYpx = mario::FileIO::getConst("DISP_Y_px");

	/* アクチュエータの初期化 */
	this->upActuator = ( unique_ptr<mario::Display::Actuator> )( new mario::Display::Actuator() );
	/* ウィンドウ初期化 */
	this->window = (unique_ptr<mario::Window>)(new mario::Window(this->screenXpx,this->screenYpx,"Main Window",false));
}

void mario::Display::oneLoop()
{
	for(auto it=this->displayedElements.begin();it!=this->displayedElements.end(); ){
		if(auto sp=it->second.lock()){
			sp->oneLoop();
			it++;
		}else{
			it = this->displayedElements.erase(it);
		}
	}
}

void mario::Display::addDisplayedElement( shared_ptr<mario::Displayed> _ptr )
{
	this->displayedElements.insert(make_pair(_ptr->getPriority(),_ptr));
}




/* ---------- アクチュエータ ---------- */

mario::Display::Actuator::Actuator()
	:zd(0.0)
{
	static int const ARDUINO_COM_NUM = FileIO::getConst("ARDUINO_COM_NUM");
	this->upPort = (unique_ptr<mario::WinRS>)( new mario::WinRS( ARDUINO_COM_NUM, 9600, ifLine::cr, "8N1", false ) );
}

void mario::Display::Actuator::moveTo( typeD _zd )
{
	this->upPort->putc1( max(0.0,_zd) );
}