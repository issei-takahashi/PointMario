#include "DeruChara.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"
#include "Eventer.h"
#include "PointBody.h"
#include "DownOutMeasure.h"
#include "MarkerDetecter.h"
#include "Cross.h"
#include "MidairObject.h"

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
	/* MeasureBasementの生成 */
	Coordinate<typeM> ret;
	cout << "MeasureBasementの初期化中..." << endl;
	DownOutMeasure base(this->MtoDMat);
	base.start();

	/* Displayの生成 */
	auto disp = Display::getInstance();
	disp->setScreenMode( true );

	/* 空中オブジェクトの生成 */
	typedef MidairObject<PointBody,Cross> Obj;

	/* 表示の生成 */
	auto img = (shared_ptr<mario::Cross>)(new Cross(mario::ColorRGB(255,0,0),10,100));
	img->displayStart();
	img->setDisplayPoint( Coordinate<typeD>(150,100,100) );
	/* ボディの生成 */
	auto body = (shared_ptr<mario::PointBody>)(new PointBody(this->shared_from_this()));
	/* オブジェクトにくっつける */
	shared_ptr<Obj> chara = (shared_ptr<Obj>)(new Obj(body,img));	

	frame_t frameCount = 0;
	while(1){
		frameCount++;
		auto ms1 = Timer::getInstance()->getms();
		/* 当たり判定 */
		if( base.collisionDetectionWithCloud(*chara->getBody(),10.0) ){
			chara->getDisplayed()->setColor(0,255,0);
		}else{
			chara->getDisplayed()->setColor(255,0,0);
		}
		/* ディスプレイの描画と移動 */
		disp->oneLoop();
		disp->moveActuatorTo(chara->getDisplayed()->getDisplayPoint().z);
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