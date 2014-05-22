#include "Experiment.h"
#include "RedClusterDetecter.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "ExperimentData.h"
#include "Cross.h"
#include "Timer.h"
#include "Eventer.h"

using namespace mario;

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
		cout << count << "回目の計測を開始します．" << endl;
		data.pR = this->inputRealPositionLoop();
		data.pM = this->measureRedPointsLoop( &data.v_redPointCenters, beforepM );
		data.pD = this->showCrossAndRegisterCrossLoop( beforepD );
		string str = utils::int2string(count);
		times(i,0,4-str.size()){
			str = string("0") + str;
		}
		string filePath = fileNameBase + "_" + str + ".xml";
		data.write(filePath,data);
		cout << filePath << "にデータを保存しました．" << endl;
		count++;
		dataList.l_exp001datas.push_back( data );
		beforepM = data.pM;
		beforepD = data.pD;
	}
	while( this->inputContinueLoop() );
	dataList.write( fileNameBase+".xml", dataList );
	cout << fileNameBase+".xml" << "にデータを保存しました．" << endl;
	dataList.writeCsv( fileNameBase+".csv" );
	cout << fileNameBase+".csv" << "にデータを保存しました．" << endl;
}

bool issei::Experiment001::inputContinueLoop()
{
	cout << "計測を終えますか？(y/n)：";
	string okStr;
	cin  >> okStr;
	if( okStr == "y" ){
		cout << "本当に終わりますか？(y/n)：";
		cin  >> okStr;
		if( okStr == "y" ){
			return false;
		}else{
			return true;
		}
	}else{
		return true;
	}
}

string issei::Experiment001::inputFileNameLoop()
{
	string buf = "";
	bool okFlag = false;
	while( okFlag == false ){
		cout << "exdata/内に保存するファイル名を入力してください：";
		cin  >> buf;
		cout << buf << " でよろしいですか？(y/n)：";
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
			cout << "pR.x(m)を入力してください：";
			cin  >> buf;
		}
		ret.x = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "pR.y(m)を入力してください：";
			cin  >> buf;
		}
		ret.y = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "pR.z(m)を入力してください：";
			cin  >> buf;
		}
		ret.z = utils::string2int(buf);
		cout << "(" << ret.x << "," << ret.y << "," << ret.z << ") でよろしいですか？(y/n)：";
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
		cout << "MeasureBasementの初期化中..." << endl;
		mario::RedClusterDetecter base;
		base.start();
		static int const RED_CENTER_TIMES = mario::FileIO::getConst("RED_CENTER_TIMES");
		cout << "赤い場所の重心を"<< RED_CENTER_TIMES <<"回計測します..." << endl;
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
				Coordinate<typeM> thisred = base.getRedCenter();
				ret += thisred;
				_pDst->push_back( thisred );
				cout << "*";
			}
		}
		cout << endl;
		ret /= RED_CENTER_TIMES;
		base.stop();
		cout << RED_CENTER_TIMES <<"回計測しました．平均値は(" << ret.x << "," << ret.y << "," << ret.z << "),移動量は" << _beforeCenter.distance(ret) << "です．" << endl;
		cout << "やり直しますか？(y/n)：";
		string againStr;
		cin  >> againStr;
		if( againStr == "y" ){
			continue;
		}else{
			break;
		}
	}
	return ret;
}

mario::Coordinate<mario::typeD> issei::Experiment001::showCrossAndRegisterCrossLoop( mario::Coordinate<mario::typeD> const & _beforepD )
{
	cout << "+マークを合わせてください" << endl;

	auto disp = mario::Display::getInstance();
	disp->setScreenMode( true );
	auto cross = (shared_ptr<mario::Cross>)(new mario::Cross());
	cross->setDisplayPoint( _beforepD );

	bool endFlag = false;
	bool keyUpdateFlag = false;
	while( endFlag == false ){
		auto ms1 = Timer::getInstance()->getms();
		if( keyUpdateFlag ){
			endFlag = mario::Eventer::getInstance()->quitEvent();
		}
		keyUpdateFlag = !keyUpdateFlag;
		auto ms2 = Timer::getInstance()->getms();
		static int const FPS = mario::FileIO::getConst("FPS");
		if( ms2 - ms1 < 1000.0/FPS ){
			disp->wait( 1000.0/FPS - ( ms2 - ms1 ) );
		}
		auto ms3 = Timer::getInstance()->getms();
	}
	return cross->getDisplayPoint();
}
