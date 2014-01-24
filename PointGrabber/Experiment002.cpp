#include "Experiment.h"
#include "ExperimentData.h"

// �_�QP->�_�QY �ւ̊􉽕ϊ������߂�
template<class T1, class T2>
boost::shared_ptr<Eigen::MatrixXd> getTranslateMatrix( mario::Experiment001DataList const & _dataList )
{
	int const Np = _dataList.l_exp001datas.size();
	using namespace Eigen;
	vector<VectorXd> P; // 3�����_�QP
	vector<VectorXd> Y; // 3�����_�QY
	MatrixXd mu_P(3,1); // P�̏d�S
	MatrixXd mu_Y(3,1); // Y�̏d�S
	MatrixXd S_PY(3,3); // P��Y�̋����U�s��

	/* �_�Q�����������āC���łɏd�S�Z�o */
	{
		if( typeid(T1) == typeid(mario::typeR) ){
			foreach(it,_dataList.l_exp001datas){
				VectorXd tmp(3);
				tmp(0) = it->pR.x;
				tmp(1) = it->pR.y;
				tmp(2) = it->pR.z;
				P.push_back( tmp );
				mu_P(0,1) += tmp(0);
				mu_P(1,1) += tmp(1);
				mu_P(2,1) += tmp(2);
			}
			mu_P /= Np;
		} else if( typeid(T1) == typeid(mario::typeM) ) {
			foreach(it,_dataList.l_exp001datas){
				VectorXd tmp(3);
				tmp(0) = it->pM.x;
				tmp(1) = it->pM.y;
				tmp(2) = it->pM.z;
				P.push_back( tmp );
				mu_P(0,1) += tmp(0);
				mu_P(1,1) += tmp(1);
				mu_P(2,1) += tmp(2);
			}
			mu_P /= Np;
		} else if( typeid(T1) == typeid(mario::typeD) ) {
			foreach(it,_dataList.l_exp001datas){
				VectorXd tmp(3);
				tmp(0) = it->pD.x;
				tmp(1) = it->pD.y;
				tmp(2) = it->pD.z;
				P.push_back( tmp );
				mu_P(0,1) += tmp(0);
				mu_P(1,1) += tmp(1);
				mu_P(2,1) += tmp(2);
			}
			mu_P /= Np;
		}

		if( typeid(T2) == typeid(mario::typeR) ) {
			foreach(it,_dataList.l_exp001datas){
				VectorXd tmp(3);
				tmp(0) = it->pR.x;
				tmp(1) = it->pR.y;
				tmp(2) = it->pR.z;
				P.push_back( tmp );
				mu_Y(0,1) += tmp(0);
				mu_Y(1,1) += tmp(1);
				mu_Y(2,1) += tmp(2);
			}
			mu_Y /= Np;
		} else if( typeid(T2) == typeid(mario::typeM) ){
			foreach(it,_dataList.l_exp001datas){
				VectorXd tmp(3);
				tmp(0) = it->pM.x;
				tmp(1) = it->pM.y;
				tmp(2) = it->pM.z;
				P.push_back( tmp );
				mu_Y(0,1) += tmp(0);
				mu_Y(1,1) += tmp(1);
				mu_Y(2,1) += tmp(2);
			}
			mu_Y /= Np;
		} else if( typeid(T2) == typeid(mario::typeD) ) {
			foreach(it,_dataList.l_exp001datas){
				VectorXd tmp(3);
				tmp(0) = it->pD.x;
				tmp(1) = it->pD.y;
				tmp(2) = it->pD.z;
				P.push_back( tmp );
				mu_Y(0,1) += tmp(0);
				mu_Y(1,1) += tmp(1);
				mu_Y(2,1) += tmp(2);
			}
			mu_Y /= Np;
		}
	}
	/* P��Y�̋����U�s��S_PY */
	{
		times(i,0,Np){
			S_PY += ( P.at(i) - mu_P ) * ( Y.at(i) - mu_Y ).transpose();
		}
		S_PY /= Np;
	}
	/*  */

}

void mario::Experiment002::experimentLoop()
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
	::getTranslateMatrix<typeR,typeD>( datas );

}

string mario::Experiment002::inputFileNameLoop()
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

