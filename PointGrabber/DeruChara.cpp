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
	once_double resolution = FileIO::getConst("OCTREE_RESOLUTION");
	once_double radius = FileIO::getConst("OCTREE_RADIUS");
	once_double K = FileIO::getConst("OCTREE_K");
	once_double distance = FileIO::getConst("SIMPLE_SEARCH_DISTANCE");

	/* MeasureBasement�̐��� */
	Coordinate<typeM> ret;
	cout << "MeasureBasement�̏�������..." << endl;
	DownOutMeasure base(this->MtoDMat);
	base.start();

	/* Display�̐��� */
	auto disp = Display::getInstance();
	disp->setScreenMode( true );

	/* �󒆃I�u�W�F�N�g�̐��� */
	typedef MidairObject<PointBody,Circle> Obj;

	/* �\���̐��� */
	auto img = Circle::makeShared(50,mario::ColorRGB(0,255,0));
	img->displayStart();
	img->setDisplayPoint( Coordinate<typeD>(150,100,100) );
	/* �{�f�B�̐��� */
	auto body = PointBody::makeShared(this->shared_from_this());
	body->setPoint(img->getDisplayPoint());
	/* �I�u�W�F�N�g�ɂ������� */
	shared_ptr<Obj> chara = (shared_ptr<Obj>)(new Obj(body,img));	

	frame_t frameCount = 0;
	while(1){
		frameCount++;
		auto ms1 = Timer::getInstance()->getms();
		/* �����蔻�� */
		base.oneLoop();
		if( base.collisionDetectionWithCloud_simple(*chara->getBody(),distance) ){
			chara->getDisplayed()->setColor(255,0,0);
		}else{
			chara->getDisplayed()->setColor(0,255,0);
		}
		/* �f�B�X�v���C�̕`��ƈړ� */
		disp->oneLoop();
		disp->moveActuatorTo(chara->getDisplayed()->getDisplayPoint().z);
		/* �I������ */
		if( Eventer::getInstance()->quitEvent() ){
			break;
		}
		/* FPS�̒��� */
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