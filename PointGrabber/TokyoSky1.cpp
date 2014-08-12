#include "TokyoSky1.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "SolidAnimation.h"
#include "DownOutMeasure.h"
#include "MarkerDetecter.h"

using namespace mario;

shared_ptr<TokyoSky1> TokyoSky1::makeShared( Eigen::Matrix4d const & _MtoD )
{
	shared_ptr<TokyoSky1> sp = (shared_ptr<TokyoSky1>)(new TokyoSky1(_MtoD));
	return sp;
}

TokyoSky1::TokyoSky1( Eigen::Matrix4d const & _MtoD )
	:Application(_MtoD)
{

}

void TokyoSky1::mainLoop()
{
	Coordinate<typeM> ret;
	cout << "MeasureBasement�̏�������..." << endl;
	//this->downOutMeasure.displayFlag = false;
	//this->downOutMeasure.start();
	MarkerDetecter detecter;
	detecter.start();
	Coordinate<typeM> rcM;
	while( rcM.x == 0 && rcM.y == 0 && rcM.z == 0 ){
		detecter.oneLoop();
		rcM = detecter.getJigCenter();
	}
	detecter.stop();
	cout << rcM.x << " " << rcM.y << " " << rcM.z << endl;
	auto rcD = rcM * this->MtoDMat;
	Coordinate<typeD> tokyoP(rcD.x+80,rcD.y+35,rcD.z-150);
	Coordinate<typeD> skyP(rcD.x-70,rcD.y+130,rcD.z-150);
	auto disp = Display::getInstance();
	disp->setScreenMode( true );
	auto tokyo = (shared_ptr<SolidAnimation>)(new SolidAnimation("image/tokyo_name/",this->shared_from_this()));
	auto sky = (shared_ptr<SolidAnimation>)(new SolidAnimation("image/sky_name/",this->shared_from_this()));
	tokyo->displayStart();
	tokyo->setDisplayPoint(tokyoP);
	sky->displayStart();
	sky->setDisplayPoint(skyP);

	while(1){
		auto ms1 = Timer::getInstance()->getms();

		/* �f�B�X�v���C�̕`��ƈړ� */
		disp->oneLoop();
		disp->moveActuatorTo(tokyo->getDisplayPoint().z);
		/* �I������ */
		if( Eventer::getInstance()->quitEvent() ){
			break;
		}
		/* FPS�̒��� */
		auto ms2 = Timer::getInstance()->getms();
		static int const FPS = FileIO::getConst("FPS");
		if( ms2 - ms1 < 1000.0/FPS ){
			Sleep( 1000.0/FPS - ( ms2 - ms1 ) );
		}
		auto ms3 = Timer::getInstance()->getms();
	}
	disp->closeWindow();

	return ;
}