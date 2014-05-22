#include "DeruChara.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "SolidAnimation.h"
#include "DownOutMeasure.h"
#include "RedClusterDetecter.h"
#include "pcl_utils.h"

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

	auto disp = Display::getInstance();
	disp->setScreenMode( true );
	auto hiyoko = (shared_ptr<SolidAnimation>)(new SolidAnimation("image/hiyoko/"));
	hiyoko->displayStart();

	while(1){
		auto ms1 = Timer::getInstance()->getms();
		/* ディスプレイの描画と移動 */
		detecter.oneLoop();
		auto rcM = detecter.getRedCenter();
		auto rcD = mario::innerProduct(this->MtoDMat,rcM);
		hiyoko->setDisplayPoint(rcD);
		disp->oneLoop();
		disp->moveActuatorTo(rcD.z);
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
	detecter.stop();
	disp->closeWindow();

	return ;
}