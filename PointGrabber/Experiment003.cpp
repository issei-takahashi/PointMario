#include "Experiment.h"
#include "RedClusterDetecter.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"

void issei::Experiment003::experimentLoop()
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasement‚Ì‰Šú‰»’†..." << endl;
	mario::RedClusterDetecter base;
	base.start();

	Eigen::Matrix4d A;
	mario::FileIO::loadTranslation( "data/MtoD.csv", A );
	bool endFlag = false;
	while( endFlag == false ){
		auto ms1 = Timer::getInstance()->getms();
		//base.oneLoop();
		auto thisRed = base.getRedCenter();
		//thisRed *= 1000;
		Eigen::Vector4d tmpv;
		tmpv(0) = thisRed.x;
		tmpv(1) = thisRed.y;
		tmpv(2) = thisRed.z;
		tmpv(3) = 1.0;
		tmpv = A*tmpv;
		mario::Coordinate<mario::typeD> pD(tmpv(0),tmpv(1),tmpv(2));
		cout << pD.x << "," << pD.y << "," << pD.z << endl;
		mario::Display::getInstance()->drawCross( pD, true );
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
