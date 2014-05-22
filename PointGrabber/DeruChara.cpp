#include "DeruChara.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "SolidAnimation.h"
#include "DownOutMeasure.h"
#include "RedClusterDetecter.h"

using namespace mario;

DeruChara::DeruChara( Eigen::Matrix4d const & _MtoD )
	:MtoDMat(_MtoD)//,downOutMeasure( DownOutMeasure(_MtoD) )
{

}

void DeruChara::mainLoop()
{
	Coordinate<typeM> ret;
	cout << "MeasureBasementの初期化中..." << endl;
	//this->downOutMeasure.displayFlag = false;
	//this->downOutMeasure.start();
	RedClusterDetecter detecter;
	detecter.start();
	Coordinate<typeR> rcR;
	while( rcR.x == 0 && rcR.y == 0 && rcR.z == 0 ){
		detecter.oneLoop();
		rcR = detecter.getRedCenter();
	}
	detecter.stop();
	cout << rcR.x << " " << rcR.y << " " << rcR.z << endl;
	auto rcD = rcR * this->MtoDMat;
	Coordinate<typeD> tokyoP(rcD.x+60,rcD.y+150,rcD.z-150);
	Coordinate<typeD> skyP(rcD.x-60,rcD.y+75,rcD.z-150);
	auto disp = Display::getInstance();
	disp->setScreenMode( true );
	//auto tokyo = (shared_ptr<SolidAnimation>)(new SolidAnimation("image/tokyo_mm/"));
	//auto sky = (shared_ptr<SolidAnimation>)(new SolidAnimation("image/sky_mm/"));
	auto tokyo = (shared_ptr<SolidAnimation>)(new SolidAnimation("image/hiyoko/"));
	auto sky = (shared_ptr<SolidAnimation>)(new SolidAnimation("image/hiyoko/"));
	tokyo->displayStart();
	tokyo->setDisplayPoint(tokyoP);
	sky->displayStart();
	sky->setDisplayPoint(skyP);

	while(1){
		auto ms1 = Timer::getInstance()->getms();

		/* ディスプレイの描画と移動 */
		disp->oneLoop();
		disp->moveActuatorTo(300-tokyo->getDisplayPoint().z);
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