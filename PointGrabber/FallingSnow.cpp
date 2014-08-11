#include "FallingSnow.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "SolidAnimation.h"
#include "DownOutMeasure.h"
#include "RedClusterDetecter.h"
#include "Circle.h"

using namespace mario;

shared_ptr<FallingSnow> FallingSnow::makeShared( Eigen::Matrix4d const & _MtoD )
{
	shared_ptr<FallingSnow> sp = (shared_ptr<FallingSnow>)(new FallingSnow(_MtoD));
	return sp;
}

FallingSnow::FallingSnow( Eigen::Matrix4d const & _MtoD )
	:Application(_MtoD)
{

}

void FallingSnow::mainLoop()
{
	Coordinate<typeM> ret;
	cout << "MeasureBasementの初期化中..." << endl;
	//this->downOutMeasure.displayFlag = false;
	//this->downOutMeasure.start();
	RedClusterDetecter detecter;
	detecter.start();
	Coordinate<typeM> rcM;
	while( rcM.x == 0 && rcM.y == 0 && rcM.z == 0 ){
		detecter.oneLoop();
		rcM = detecter.getRedCenter();
	}
	detecter.stop();
	cout << rcM.x << " " << rcM.y << " " << rcM.z << endl;
	auto rcD = rcM * this->MtoDMat;

	auto disp = Display::getInstance();
	disp->setScreenMode( true );
	disp->moveActuatorTo(rcD.z);

	list<shared_ptr<SolidAnimation> > snows;

	frame_t frameCount = 0;
	while(1){
		frameCount++;
		auto ms1 = Timer::getInstance()->getms();
		/* 雪の生成 */
		if( frameCount % 10 == 0 ){
			auto circle = (shared_ptr<SolidAnimation>)(new SolidAnimation("image/circle/",this->shared_from_this()));
			circle->displayStart();
			double ran = utils::random(-150,150);
			circle->setDisplayPoint( Coordinate<typeD>(rcD.x+ran,rcD.y+300,rcD.z) );
			circle->setVelocity(Eigen::Vector3d(0,-1,0));
			snows.push_back(circle);
		}
		/* 雪の動き */
		foreach(it,snows){
			auto dp = (*it)->getDisplayPoint();
			if( rcD.x-40.0 < dp.x && dp.x < rcD.x+40.0 ){
				if( dp.y < rcD.y+40.0 ){
					(*it)->setVelocity(Eigen::Vector3d::Zero());
				}
			}else{
				if( dp.y < rcD.y - 15 ){
					(*it)->setVelocity(Eigen::Vector3d::Zero());
				}		
			}
		}
		/* ディスプレイの描画と移動 */
		disp->oneLoop();
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