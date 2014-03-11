#include "Experiment.h"
#include "RedClusterDetecter.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "ExperimentData.h"

void issei::Experiment001::experimentLoop()
{
	static int const DISP_X_mm = mario::FileIO::getConst("DISP_X_mm");
	static int const DISP_Y_mm = mario::FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = mario::FileIO::getConst("DISP_Z_mm");
	string fileNameBase = string("exdata/") + this->inputFileNameLoop();
	int count = 1;
	issei::Experiment001DataList dataList;
	mario::Coordinate<mario::typeM> beforepM;
	mario::Coordinate<mario::typeD> beforepD(DISP_X_mm/2,DISP_Y_mm/2,0.0);
	do{
		issei::Experiment001Data data;
		cout << count << "��ڂ̌v�����J�n���܂��D" << endl;
		data.pR = this->inputRealPositionLoop();
		data.pM = this->measureRedPointsLoop( &data.v_redPointCenters, beforepM );
		data.pD = this->showCrossAndRegisterCrossLoop( beforepD );
		string str = utils::int2string(count);
		times(i,0,4-str.size()){
			str = string("0") + str;
		}
		string filePath = fileNameBase + "_" + str + ".xml";
		data.write(filePath,data);
		cout << filePath << "�Ƀf�[�^��ۑ����܂����D" << endl;
		count++;
		dataList.l_exp001datas.push_back( data );
		beforepM = data.pM;
		beforepD = data.pD;
	}
	while( this->inputContinueLoop() );
	dataList.write( fileNameBase+".xml", dataList );
	cout << fileNameBase+".xml" << "�Ƀf�[�^��ۑ����܂����D" << endl;
	dataList.writeCsv( fileNameBase+".csv" );
	cout << fileNameBase+".csv" << "�Ƀf�[�^��ۑ����܂����D" << endl;
}

bool issei::Experiment001::inputContinueLoop()
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

string issei::Experiment001::inputFileNameLoop()
{
	string buf = "";
	bool okFlag = false;
	while( okFlag == false ){
		cout << "exdata/���ɕۑ�����t�@�C��������͂��Ă��������F";
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

mario::Coordinate<mario::typeR> issei::Experiment001::inputRealPositionLoop()
{
	mario::Coordinate<mario::typeR> ret;
	bool is_pR_ok = false;
	while( is_pR_ok == false ){
		string buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "pR.x(m)����͂��Ă��������F";
			cin  >> buf;
		}
		ret.x = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "pR.y(m)����͂��Ă��������F";
			cin  >> buf;
		}
		ret.y = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "pR.z(m)����͂��Ă��������F";
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

mario::Coordinate<mario::typeM> issei::Experiment001::measureRedPointsLoop( vector<mario::Coordinate<mario::typeM> > * _pDst, mario::Coordinate<mario::typeM> const & _beforeCenter )
{
	mario::Coordinate<mario::typeM> ret;
	while(1){
		ret = mario::Coordinate<mario::typeM>(0,0,0);
		cout << "MeasureBasement�̏�������..." << endl;
		mario::RedClusterDetecter base;
		base.start();
		static int const RED_CENTER_TIMES = mario::FileIO::getConst("RED_CENTER_TIMES");
		cout << "�Ԃ��ꏊ�̏d�S��"<< RED_CENTER_TIMES <<"��v�����܂�..." << endl;
		int measureCount = 0;
		times(i,0,RED_CENTER_TIMES){
			cout << "-" ;
		}
		cout << endl;
		while( measureCount < RED_CENTER_TIMES ){
			base.oneLoop();
			int mc = base.getMeasureCount();
			if( mc > measureCount ){
				measureCount = mc;
				auto thisred = base.getRedCenter();
				ret += thisred;
				_pDst->push_back( thisred );
				cout << "*";
			}
		}
		cout << endl;
		ret /= RED_CENTER_TIMES;
		base.stop();
		cout << RED_CENTER_TIMES <<"��v�����܂����D���ϒl��(" << ret.x << "," << ret.y << "," << ret.z << "),�ړ��ʂ�" << _beforeCenter.distance(ret) << "�ł��D" << endl;
		cout << "��蒼���܂����H(y/n)�F";
		string againStr;
		cin  >> againStr;
		if( againStr == "y" ){
			continue;
		}
		else{
			break;
		}
	}
	return ret;
}

mario::Coordinate<mario::typeD> issei::Experiment001::showCrossAndRegisterCrossLoop( mario::Coordinate<mario::typeD> const & _beforepD )
{
	static int const DISP_X_mm = mario::FileIO::getConst("DISP_X_mm");
	static int const DISP_Y_mm = mario::FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = mario::FileIO::getConst("DISP_Z_mm");
	static int const DISP_X_px = mario::FileIO::getConst("DISP_X_px");
	static int const DISP_Y_px = mario::FileIO::getConst("DISP_Y_px");
	cout << "+�}�[�N�����킹�Ă�������" << endl;
	static mario::Display disp( DISP_X_mm, DISP_Y_mm, DISP_X_px, DISP_Y_px );
	disp.start();
	disp.setScreenMode( true );
	disp.set_crossPos( _beforepD );
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
		static int const FPS = mario::FileIO::getConst("FPS");
		if( ms2 - ms1 < 1000.0/FPS ){
			disp.wait( 1000.0/FPS - ( ms2 - ms1 ) );
		}
		auto ms3 = mario::DisplayTimer::getTime();
	}
	disp.stop();
	return disp.get_crossPos();
}
