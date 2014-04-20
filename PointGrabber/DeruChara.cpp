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
	cout << "MeasureBasementの初期化中..." << endl;
	this->downOutMeasure.start();

	auto disp = mario::Display::getInstance();
	disp->setScreenMode( true );
	auto chara = mario::SolidAnimation::makeShared("image/hiyoko/");
	chara->displayStart();

	while(1){
		auto ms1 = Timer::getInstance()->getms();
		/* キャラクタの移動処理 */
		auto vel = chara->getVelocity();
		vel.y() -= 5; // 下におちる
		if( this->downOutMeasure.collisionDetectionWithCloud( *chara ) ){
			vel.y() *= -1; // ぶつかったらy方向に反射
		}
		chara->setVelocity(vel);
		/* ディスプレイの描画と移動 */
		disp->oneLoop();
		disp->moveActuatorTo(chara->getDisplayPoint().z);
		/* 終了処理 */
		if( mario::Eventer::getInstance()->quitEvent() ){
			break;
		}
		/* FPSの調整 */
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