#include "TokyoSky2.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "Animation.h"
#include "DownOutMeasure.h"
#include "MarkerDetecter.h"

using namespace mario;

shared_ptr<TokyoSky2> TokyoSky2::makeShared( Eigen::Matrix4d const & _MtoD )
{
	shared_ptr<TokyoSky2> sp = (shared_ptr<TokyoSky2>)(new TokyoSky2(_MtoD));
	return sp;
}

TokyoSky2::TokyoSky2( Eigen::Matrix4d const & _MtoD )
	:Application(_MtoD)
{

}

void TokyoSky2::mainLoop()
{
	Coordinate<typeM> ret;
	cout << "MeasureBasementの初期化中..." << endl;
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
	Coordinate<typeD> tokyoP(rcD.x,rcD.y,rcD.z);
	Coordinate<typeD> skyP(rcD.x,rcD.y,rcD.z);
	auto disp = Display::getInstance();
	disp->setScreenMode( true );
	auto tokyo = Animation::makeShared("image/hiyoko/");
	auto sky = Animation::makeShared("image/hiyoko/");
	tokyo->displayStart();
	tokyo->setDisplayPoint(tokyoP);
	sky->displayStart();
	sky->setDisplayPoint(skyP);

	while(1){
		auto ms1 = Timer::getInstance()->getms();

		/* ディスプレイの描画と移動 */
		disp->oneLoop();
		disp->moveActuatorTo(tokyo->getDisplayPoint().z);
		/* 終了処理 */
		if( Eventer::getInstance()->quitEvent() ){
			break;
		}
		/* FPSの調整 */
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