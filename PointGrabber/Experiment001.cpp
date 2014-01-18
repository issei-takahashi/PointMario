#include "Experiment.h"
#include "MeasureBasement.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"

void mario::Experiment001::experimentLoop()
{
	string fileNameBase = this->inputFileNameLoop();
	int count = 1;
	mario::Experiment001::Experiment001DataList dataList;
	do{
		mario::Experiment001::Experiment001Data data;
		cout << count << "��ڂ̌v�����J�n���܂��D" << endl;
		data.pR = this->inputRealPositionLoop();
		data.pM = this->measureRedPointsLoop( &data.v_redPointCenters );
		data.pD = this->showCrossAndRegisterCrossLoop();
		string str = utils::int2string(count);
		times(i,0,4-str.size()){
			str = string("0") + str;
		}
		string filePath = fileNameBase + "_" + str + ".xml";
		data.write(filePath,data);
		cout << filePath << "�Ƀf�[�^��ۑ����܂����D" << endl;
		count++;
		dataList.l_exp001datas.push_back( data );
	}
	while( this->inputContinueLoop() );
	string filePath = fileNameBase + ".xml";
	dataList.write(filePath,dataList);
	cout << filePath << "�Ƀf�[�^��ۑ����܂����D" << endl;
}

bool mario::Experiment001::inputContinueLoop()
{
	cout << "�v�����I���܂����H(y/n)�F";
	string okStr;
	cin  >> okStr;
	if( okStr == "y" ){
		cout << "�{���ɏI���܂����H(y/n)�F";
		cin  >> okStr;
		if( okStr == "y" ){
			return false;
		}
		else{
			return true;
		}
	}
	else{
		return true;
	}
}

string mario::Experiment001::inputFileNameLoop()
{
	string buf = "";
	bool okFlag = false;
	while( okFlag == false ){
		cout << "�t�@�C��������͂��Ă��������F";
		cin  >> buf;
		cout << buf << " �ł�낵���ł����H(y/n)�F";
		string okStr;
		cin  >> okStr;
		if( okStr == "y" ){
			okFlag = true;
		}
	}
	return buf;
}

mario::Coordinate<mario::typeR> mario::Experiment001::inputRealPositionLoop()
{
	mario::Coordinate<typeR> ret;
	bool is_pR_ok = false;
	while( is_pR_ok == false ){
		string buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "ret.x(mm)����͂��Ă��������F";
			cin  >> buf;
		}
		ret.x = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "ret.y(mm)����͂��Ă��������F";
			cin  >> buf;
		}
		ret.y = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "ret.z(mm)����͂��Ă��������F";
			cin  >> buf;
		}
		ret.z = utils::string2int(buf);
		cout << "(" << ret.x << "," << ret.y << "," << ret.z << ") �ł�낵���ł����H(y/n)�F";
		string okStr;
		cin  >> okStr;
		if( okStr == "y" ){
			is_pR_ok = true;
		}
	}
	return ret;
}

mario::Coordinate<mario::typeM> mario::Experiment001::measureRedPointsLoop( vector<Coordinate<typeM> > * _pDst )
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasement�̏�������..." << endl;
	mario::MeasureBasement base;
	base.start();
	static int const RED_CENTER_TIMES = FileIO::getConst("RED_CENTER_TIMES");
	cout << "�Ԃ��ꏊ�̏d�S��"<< RED_CENTER_TIMES <<"��v�����܂�..." << endl;
	int measureCount = 0;
	while( measureCount < RED_CENTER_TIMES ){
		base.oneLoop();
		int mc = base.getMeasureCount();
		if( mc > measureCount ){
			measureCount = mc;
			_pDst->push_back( base.getRedCenter() );
		}
	}
	base.stop();
	cout << "�Ԃ��ꏊ�̏d�S"<< RED_CENTER_TIMES <<"��v�����܂����D���ϒl��(" << "," << ret.x << "," << ret.y << "," << ret.z << "�ł��D" << endl;
	return ret;
}

mario::Coordinate<mario::typeD> mario::Experiment001::showCrossAndRegisterCrossLoop()
{
	static int const DISP_X_mm = FileIO::getConst("DISP_X_mm");
	static int const DISP_Y_mm = FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = FileIO::getConst("DISP_Z_mm");
	static int const DISP_X_px = FileIO::getConst("DISP_X_px");
	static int const DISP_Y_px = FileIO::getConst("DISP_Y_px");
	cout << "�v�������ʒu��+�}�[�N���o���܂��D����Ă���������L�[��+���ړ����č��킹�Ă��������D" << endl;
	mario::Display disp( DISP_X_mm, DISP_Y_mm, DISP_X_px, DISP_Y_px );
	disp.start();
	disp.changeScreenMode();
	disp.set_crossPos( Coordinate<typeD>(DISP_X_mm/2,DISP_Y_mm/2,DISP_Z_mm/2) );
	bool endFlag = false;
	bool keyUpdateFlag = false;
	while( endFlag == false ){
		auto ms1 = mario::DisplayTimer::getTime();
		if( keyUpdateFlag ){
			endFlag = disp.keyInputEvent1();
		}
		keyUpdateFlag = !keyUpdateFlag;
		disp.drawCross( disp.get_crossPos(), true );
		disp.quitEvent();
		auto ms2 = mario::DisplayTimer::getTime();
		static int const FPS = FileIO::getConst("FPS");
		if( ms2 - ms1 < 1000.0/FPS ){
			disp.wait( 1000.0/FPS - ( ms2 - ms1 ) );
		}
		auto ms3 = mario::DisplayTimer::getTime();
	}
	disp.stop();
	return disp.get_crossPos();
}
