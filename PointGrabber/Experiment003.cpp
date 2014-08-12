#include "Experiment.h"
#include "MarkerDetecter.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "Cross.h"

issei::Experiment003::Experiment003( string const & _MDmatFilePath )
	:MDfilePath(_MDmatFilePath)
{

}

void issei::Experiment003::experimentLoop()
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasement‚Ì‰Šú‰»’†..." << endl;
	mario::MarkerDetecter base;
	base.start();

	Eigen::Matrix4d A;
	mario::FileIO::loadMatrix( this->MDfilePath, A );
	bool endFlag = false;
	auto cross = (shared_ptr<mario::Cross>)(new mario::Cross());
	cross->displayStart();

	while( endFlag == false ){
		auto ms1 = Timer::getInstance()->getms();
		//base.oneLoop();
		auto thisRed = base.getJigCenter();
		//thisRed *= 1000;
		Eigen::Vector4d tmpv;
		tmpv(0) = thisRed.x;
		tmpv(1) = thisRed.y;
		tmpv(2) = thisRed.z;
		tmpv(3) = 1.0;
		tmpv = A*tmpv;
		mario::Coordinate<mario::typeD> pD(tmpv(0),tmpv(1),tmpv(2));
		cout << pD.x << "," << pD.y << "," << pD.z << endl;
		cross->setDisplayPoint(pD);
		mario::Display::getInstance()->moveActuatorTo(pD.z);
		if( mario::Eventer::getInstance()->quitEvent() ){
			endFlag = true;
		}
		auto ms2 = Timer::getInstance()->getms();
		static int const FPS = mario::FileIO::getConst("FPS");
		if( ms2 - ms1 < 1000.0/FPS ){
			Sleep( 1000.0/FPS - ( ms2 - ms1 ) );
		}
		auto ms3 = Timer::getInstance()->getms();
	}
	base.stop();
	return ;
}
