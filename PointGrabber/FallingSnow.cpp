#include "FallingSnow.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "Animation.h"
#include "DownOutMeasure.h"
#include "MarkerDetecter.h"
#include "Circle.h"
#include "MidairObject.h"
#include "PointBody.h"

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

	auto disp = Display::getInstance();
	disp->setScreenMode( true );
	cout << "rcD : " << rcD.x << "  " << rcD.y << "  " << rcD.z << endl;
	disp->moveActuatorTo(rcD.z);

	typedef MidairObject<PointBody,Animation> Obj;
	list<shared_ptr<Obj> > snows;

	frame_t frameCount = 0;
	while(1){
		frameCount++;
		auto ms1 = Timer::getInstance()->getms();
		/* 雪の生成 */
		if( frameCount % 10 == 0 ){
			// ボディ
			double ran = utils::random(-300,300);
			auto body = (shared_ptr<PointBody>)(new PointBody(this->shared_from_this()));
			body->setPoint( Coordinate<typeD>(rcD.x+ran,rcD.y+300,rcD.z) );
			body->setVelocity(Eigen::Vector3d(0,-1,0));
			// 表示
			auto circle = (shared_ptr<Animation>)(new Animation("image/circle/"));
			circle->displayStart();
			circle->setDisplayPoint( body->getPoint() );
			// 空中オブジェクトにする
			snows.push_back(shared_ptr<Obj>(new Obj(body,circle) ));
		}
		/* 雪の動き */
		foreach(it,snows){
			(*it)->getBody()->oneLoop();
			auto dp = (*it)->getBody()->getPoint();
			(*it)->getDisplayed()->setDisplayPoint(dp);
			if( rcD.x-40.0 < dp.x && dp.x < rcD.x+40.0 ){
				if( dp.y < rcD.y+40.0 ){
					(*it)->getBody()->setVelocity(Eigen::Vector3d::Zero());
				}
			}else{
				if( dp.y < rcD.y - 15 ){
					(*it)->getBody()->setVelocity(Eigen::Vector3d::Zero());
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