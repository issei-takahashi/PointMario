#include "DeruChara.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "SolidAnimation.h"
#include "DownOutMeasure.h"

DeruChara::DeruChara( Eigen::Matrix4d const & _MtoD )
	:MtoDMat(_MtoD),downOutMeasure( mario::DownOutMeasure(_MtoD) )
{

}

void DeruChara::mainLoop()
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasement‚Ì‰Šú‰»’†..." << endl;
	this->downOutMeasure.start();

	bool endFlag = false;
	auto chara = mario::SolidAnimation::makeShared("image/hiyoko/");
	chara->displayStart();

	while( endFlag == false ){
		auto ms1 = Timer::getInstance()->getms();
		//this->downOutMeasure.oneLoop();
		this->downOutMeasure.collisionDetectionWithCloud( *chara );
		mario::Display::getInstance()->moveActuatorTo(chara->getDisplayPoint().z);
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
	this->downOutMeasure.stop();
	return ;
}