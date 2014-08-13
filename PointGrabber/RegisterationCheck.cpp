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
	/* �}�[�J���o��̐��� */
	cout << "MarkerDetecter�̏�������..." << endl;
	MarkerDetecter detecter;
	detecter.start();

	/* �f�B�X�v���C�̐��� */
	auto disp = Display::getInstance();
	disp->setScreenMode( true );

	/* �{�̐��� */
	mario::ColorRGB color(255,0,0);
	auto cross = (shared_ptr<mario::Cross>)(new mario::Cross(color,10,100));
	cross->displayStart();

	frame_t frameCount = 0;
	while(1){
		frameCount++;
		auto ms1 = Timer::getInstance()->getms();
		/* ����S�ʒu�̎擾 */
		detecter.oneLoop();
		Coordinate<typeM> rcM = detecter.getJigCenter();
		auto rcD = rcM * this->MtoDMat;
		/* �{������S�Ɉړ� */
		cross->setDisplayPoint( rcD );
		disp->moveActuatorTo(rcD.z); // �A�N�`���G�[�^
		/* �f�B�X�v���C�̕`��ƈړ� */
		disp->oneLoop();

		/* �I���C�x���g */
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
	detecter.stop();
	disp->closeWindow();

	return ;
}