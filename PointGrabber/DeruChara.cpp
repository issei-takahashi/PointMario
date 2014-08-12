#include "DeruChara.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "SolidAnimation.h"
#include "DownOutMeasure.h"
#include "MarkerDetecter.h"

using namespace mario;

shared_ptr<DeruChara> DeruChara::makeShared( Eigen::Matrix4d const & _MtoD )
{
	shared_ptr<DeruChara> sp = (shared_ptr<DeruChara>)(new DeruChara(_MtoD));
	return sp;
}

DeruChara::DeruChara( Eigen::Matrix4d const & _MtoD )
	:Application(_MtoD)
{

}

void DeruChara::mainLoop()
{
	Coordinate<typeM> ret;
	cout << "MeasureBasementの初期化中..." << endl;
	DownOutMeasure base(this->MtoDMat);
	base.start();

	auto disp = Display::getInstance();
	disp->setScreenMode( true );

	auto hiyoko = (shared_ptr<SolidAnimation>)(new SolidAnimation("image/hiyoko/",this->shared_from_this()));
	hiyoko->displayStart();
	hiyoko->setDisplayPoint( Coordinate<typeD>(0,20,0) );
	hiyoko->setVelocity(Eigen::Vector3d(1,1,1));

	frame_t frameCount = 0;
	while(1){
		frameCount++;
		auto ms1 = Timer::getInstance()->getms();
		/* キャラクターの移動 */
		auto thisVel = hiyoko->getVelocity();
		bool edgeHit = false;
		auto dp = hiyoko->getDisplayPoint();
		if( dp.x < 0 ){
			thisVel.x() = abs(thisVel.x());
			edgeHit = true;
		}else if( dp.x > disp->getScreenXmm() ){
			thisVel.x() = -abs(thisVel.x());
			edgeHit = true;
		}
		if( dp.y < 20 ){
			thisVel.y() = abs(thisVel.y());
			edgeHit = true;
		}else if( dp.y > disp->getScreenYmm() ){
			thisVel.y() = -abs(thisVel.y());
			edgeHit = true;
		}
		if( dp.z < 45 ){
			thisVel.z() = abs(thisVel.z());
			edgeHit = true;
		}else if( dp.z > 300 ){
			thisVel.z() = -abs(thisVel.z());
			edgeHit = true;
		}
		if( edgeHit ){
			hiyoko->setVelocity(thisVel);
		}else{ /* 当たり判定 */
			if( base.collisionDetectionWithCloud(*hiyoko,10.0) ){
				hiyoko->setVelocity(-thisVel);
			}
		}
		hiyoko->oneLoop();
		/* ディスプレイの描画と移動 */
		disp->oneLoop();
		disp->moveActuatorTo(hiyoko->getDisplayPoint().z);
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