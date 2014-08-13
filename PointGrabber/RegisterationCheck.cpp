#include "RegisterationCheck.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "SolidAnimation.h"
#include "DownOutMeasure.h"
#include "MarkerDetecter.h"
#include "Cross.h"


using namespace mario;

shared_ptr<RegisterationCheck> RegisterationCheck::makeShared( Eigen::Matrix4d const & _MtoD )
{
	shared_ptr<RegisterationCheck> sp = (shared_ptr<RegisterationCheck>)(new RegisterationCheck(_MtoD));
	return sp;
}

RegisterationCheck::RegisterationCheck( Eigen::Matrix4d const & _MtoD )
	:Application(_MtoD)
{

}

void RegisterationCheck::mainLoop()
{
	/* マーカ検出器の生成 */
	cout << "MarkerDetecterの初期化中..." << endl;
	MarkerDetecter detecter;
	detecter.start();

	/* ディスプレイの生成 */
	auto disp = Display::getInstance();
	disp->setScreenMode( true );

	/* ＋の生成 */
	mario::ColorRGB color(255,0,0);
	auto cross = (shared_ptr<mario::Cross>)(new mario::Cross(color,10,100));
	cross->displayStart();

	frame_t frameCount = 0;
	while(1){
		frameCount++;
		auto ms1 = Timer::getInstance()->getms();
		/* 治具中心位置の取得 */
		detecter.oneLoop();
		Coordinate<typeM> rcM = detecter.getJigCenter();
		auto rcD = rcM * this->MtoDMat;
		/* ＋を治具中心に移動 */
		cross->setDisplayPoint( rcD );
		disp->moveActuatorTo(rcD.z); // アクチュエータ
		/* ディスプレイの描画と移動 */
		disp->oneLoop();

		/* 終了イベント */
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