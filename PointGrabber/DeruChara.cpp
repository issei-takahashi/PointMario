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
		auto pos = chara->getDisplayPoint();
		if(pos.x<0) vel.x() = abs(vel.x());
		if(pos.y<0) vel.y() = abs(vel.y());
		if(pos.z<0) vel.z() = abs(vel.z());
		if(pos.x>300) vel.x() = -abs(vel.x());
		if(pos.y>300) vel.y() = -abs(vel.y());
		if(pos.z>300) vel.z() = -abs(vel.z());
		pos.x = min(300.0,max(0.0,pos.x));
		pos.y = min(300.0,max(0.0,pos.y));
		pos.z = min(300.0,max(0.0,pos.z));
		chara->setDisplayPoint(pos);
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