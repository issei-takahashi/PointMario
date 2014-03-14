#include "Display.h"
#include "Timer.h"
#include "Surface.h"
#include "FileIO.h"
#include "WinRS.h"
#include "utils.h"
#include "Displayed.h"

static std::string const SCREEN_CAPTION = "SDL window test";

mario::Display::Display( int _scrXmm, int _scrYmm, int _scrXpx, int _scrYpx )
	:screenXmm(_scrXmm), screenYmm(_scrYmm), screenXpx(_scrXpx), screenYpx(_scrYpx)
{
	/* アクチュエータの初期化 */
	this->upActuator = ( unique_ptr<mario::Display::Actuator> )( new mario::Display::Actuator() );
	 
}

void mario::Display::oneLoop()
{
	foreach(it,this->displayedElements){
		it->second->oneLoop();
	}
}

void mario::Display::addDisplayedElement( shared_ptr<Displayed> _ptr )
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