#include "Experiment.h"
#include "ExperimentData.h"


void mario::Experiment1001::experimentLoop()
{
	Experiment001DataList datas;
	while(1){
		string filePath = string("exdata/") + this->inputFileNameLoop() + ".xml";
		try{
			datas.read( filePath, datas );
			cout << filePath+"��ǂݍ��݂܂����D" << endl;
			break;
		}
		catch(...){
			cout << filePath+"���ǂݍ��߂܂���ł����D�t�@�C�������m�F���ĉ������D" << endl;
			continue;
		}
	}
	{
		cout << "�{���ɏC�����܂����H(y/n)�F";
		string okStr;
		cin  >> okStr;
		if( okStr == "y" ){
			foreach(it,datas.l_exp001datas){
				it->pM *= 1000;
				it->pD.z *= -1.0;
			}
		}
	}
	cout << "�C����̃t�@�C��������͂��Ă��������F";
	string newFilePath;
	cin >> newFilePath;
	datas.write(string("exdata/")+newFilePath+".xml",datas);
}

string mario::Experiment1001::inputFileNameLoop()
{
	string buf = "";
	bool okFlag = false;
	while( okFlag == false ){
		cout << "exdata/������ǂݍ���xml�t�@�C����(�g���q����)����͂��Ă��������F";
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
