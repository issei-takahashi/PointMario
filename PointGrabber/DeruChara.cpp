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
	cout << "MeasureBasement�̏�������..." << endl;
	this->downOutMeasure.start();

	auto disp = mario::Display::getInstance();
	disp->setScreenMode( true );
	auto chara = mario::SolidAnimation::makeShared("image/hiyoko/");
	chara->displayStart();

	while(1){
		auto ms1 = Timer::getInstance()->getms();
		/* �L�����N�^�̈ړ����� */
		auto vel = chara->getVelocity();
		vel.y() -= 5; // ���ɂ�����
		if( this->downOutMeasure.collisionDetectionWithCloud( *chara ) ){
			vel.y() *= -1; // �Ԃ�������y�����ɔ���
		}
		chara->setVelocity(vel);
		/* �f�B�X�v���C�̕`��ƈړ� */
		disp->oneLoop();
		disp->moveActuatorTo(chara->getDisplayPoint().z);
		/* �I������ */
		if( mario::Eventer::getInstance()->quitEvent() ){
			break;
		}
		/* FPS�̒��� */
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