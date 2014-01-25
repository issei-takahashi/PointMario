#include "Experiment.h"
#include "ExperimentData.h"


void mario::Experiment1001::experimentLoop()
{
	Experiment001DataList datas;
	while(1){
		string filePath = string("exdata/") + this->inputFileNameLoop() + ".xml";
		try{
			datas.read( filePath, datas );
			cout << filePath+"を読み込みました．" << endl;
			break;
		}
		catch(...){
			cout << filePath+"が読み込めませんでした．ファイル名を確認して下さい．" << endl;
			continue;
		}
	}
	{
		cout << "本当に修正しますか？(y/n)：";
		string okStr;
		cin  >> okStr;
		if( okStr == "y" ){
			foreach(it,datas.l_exp001datas){
				it->pM *= 1000;
				it->pD.z *= -1.0;
			}
		}
	}
	cout << "修正後のファイル名を入力してください：";
	string newFilePath;
	cin >> newFilePath;
	datas.write(string("exdata/")+newFilePath+".xml",datas);
}

string mario::Experiment1001::inputFileNameLoop()
{
	string buf = "";
	bool okFlag = false;
	while( okFlag == false ){
		cout << "exdata/内から読み込むxmlファイル名(拡張子抜き)を入力してください：";
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
