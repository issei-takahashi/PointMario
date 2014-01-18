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
		cout << count << "回目の計測を開始します．" << endl;
		data.pR = this->inputRealPositionLoop();
		data.pM = this->measureRedPointsLoop( &data.v_redPointCenters );
		data.pD = this->showCrossAndRegisterCrossLoop();
		string str = utils::int2string(count);
		times(i,0,4-str.size()){
			str = string("0") + str;
		}
		string filePath = fileNameBase + "_" + str + ".xml";
		data.write(filePath,data);
		cout << filePath << "にデータを保存しました．" << endl;
		count++;
		dataList.l_exp001datas.push_back( data );
	}
	while( this->inputContinueLoop() );
	string filePath = fileNameBase + ".xml";
	dataList.write(filePath,dataList);
	cout << filePath << "にデータを保存しました．" << endl;
}

bool mario::Experiment001::inputContinueLoop()
{
	cout << "計測を終えますか？(y/n)：";
	string okStr;
	cin  >> okStr;
	if( okStr == "y" ){
		cout << "本当に終わりますか？(y/n)：";
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
		cout << "ファイル名を入力してください：";
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

mario::Coordinate<mario::typeR> mario::Experiment001::inputRealPositionLoop()
{
	mario::Coordinate<typeR> ret;
	bool is_pR_ok = false;
	while( is_pR_ok == false ){
		string buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "ret.x(mm)を入力してください：";
			cin  >> buf;
		}
		ret.x = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "ret.y(mm)を入力してください：";
			cin  >> buf;
		}
		ret.y = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "ret.z(mm)を入力してください：";
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

mario::Coordinate<mario::typeM> mario::Experiment001::measureRedPointsLoop( vector<Coordinate<typeM> > * _pDst )
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasementの初期化中..." << endl;
	mario::MeasureBasement base;
	base.start();
	static int const RED_CENTER_TIMES = FileIO::getConst("RED_CENTER_TIMES");
	cout << "赤い場所の重心を"<< RED_CENTER_TIMES <<"回計測します..." << endl;
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
	cout << "赤い場所の重心"<< RED_CENTER_TIMES <<"回計測しました．平均値は(" << "," << ret.x << "," << ret.y << "," << ret.z << "です．" << endl;
	return ret;
}

mario::Coordinate<mario::typeD> mario::Experiment001::showCrossAndRegisterCrossLoop()
{
	static int const DISP_X_mm = FileIO::getConst("DISP_X_mm");
	static int const DISP_Y_mm = FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = FileIO::getConst("DISP_Z_mm");
	static int const DISP_X_px = FileIO::getConst("DISP_X_px");
	static int const DISP_Y_px = FileIO::getConst("DISP_Y_px");
	cout << "計測した位置に+マークを出します．ずれていたら方向キーで+を移動して合わせてください．" << endl;
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
