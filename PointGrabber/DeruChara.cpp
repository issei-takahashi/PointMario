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
#include "Circle.h"
#include "MidairChara.h"

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
	string ans;
	cout << "計測データを表示しますか？(y/n)" << endl;
	cin >> ans;
	bool showFlag = (ans=="y");
	/* MeasureBasementの生成 */
	Coordinate<typeM> ret;
	cout << "MeasureBasementの初期化中..." << endl;
	auto base = DownOutMeasure::makeShared(this->MtoDMat);
	base->displayFlag = showFlag;
	base->start();

	/* Displayの生成 */
	auto disp = Display::getInstance();
	disp->setScreenMode( true );

	/* 空中オブジェクトの生成 */
	auto chara = MidairChara::makeShared(this->shared_from_this(),base,"image/hiyoko/");
	chara->displayStart();
	chara->setPoint(Coordinate<typeD>(150,150,150));
	chara->setDisplayPoint(chara->getPoint());

	frame_t frameCount = 0;
	while(1){
		frameCount++;
		auto ms1 = Timer::getInstance()->getms();
		base->oneLoop();

		/* ディスプレイの描画と移動 */
		disp->oneLoop();
		disp->moveActuatorTo(chara->getDisplayPoint().z);
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