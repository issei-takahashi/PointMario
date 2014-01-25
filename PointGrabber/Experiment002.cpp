#include "Experiment.h"
#include "ExperimentData.h"

// 点群P->点群Y への幾何変換を求める
template<class T1, class T2>
boost::shared_ptr<Eigen::MatrixXd> getTranslateMatrix( mario::Experiment001DataList const & _dataList )
{
	int const Np = _dataList.l_exp001datas.size();
	using namespace Eigen;
	vector<Vector3d> P; // 3次元点群P
	vector<Vector3d> Y; // 3次元点群Y
	Vector3d mu_P = Vector3d::Zero(); // Pの重心
	Vector3d mu_Y = Vector3d::Zero(); // Yの重心
	Matrix3d S_PY = Matrix3d::Zero(); // PとYの共分散行列
	Matrix3d S_PY_inv = Matrix3d::Zero(); // S_PYの転置
	Vector4d q_R = Vector4d::Zero();

	/* 点群を初期化して，ついでに重心算出 */
	{
		// P
		if( typeid(T1) == typeid(mario::typeR) ){
			foreach(it,_dataList.l_exp001datas){
				Vector3d tmp;
				tmp(0) = it->pR.x;
				tmp(1) = it->pR.y;
				tmp(2) = it->pR.z;
				P.push_back( tmp );
				mu_P(0) += tmp(0);
				mu_P(1) += tmp(1);
				mu_P(2) += tmp(2);
			}
			mu_P /= Np;
		} else if( typeid(T1) == typeid(mario::typeM) ) {
			foreach(it,_dataList.l_exp001datas){
				Vector3d tmp;
				tmp(0) = it->pM.x;
				tmp(1) = it->pM.y;
				tmp(2) = it->pM.z;
				P.push_back( tmp );
				mu_P(0) += tmp(0);
				mu_P(1) += tmp(1);
				mu_P(2) += tmp(2);
			}
			mu_P /= Np;
		} else if( typeid(T1) == typeid(mario::typeD) ) {
			foreach(it,_dataList.l_exp001datas){
				Vector3d tmp;
				tmp(0) = it->pD.x;
				tmp(1) = it->pD.y;
				tmp(2) = it->pD.z;
				P.push_back( tmp );
				mu_P(0) += tmp(0);
				mu_P(1) += tmp(1);
				mu_P(2) += tmp(2);
			}
			mu_P /= Np;
		}

		// Y
		if( typeid(T2) == typeid(mario::typeR) ) {
			foreach(it,_dataList.l_exp001datas){
				Vector3d tmp;
				tmp(0) = it->pR.x;
				tmp(1) = it->pR.y;
				tmp(2) = it->pR.z;
				Y.push_back( tmp );
				mu_Y(0) += tmp(0);
				mu_Y(1) += tmp(1);
				mu_Y(2) += tmp(2);
			}
			mu_Y /= Np;
		} else if( typeid(T2) == typeid(mario::typeM) ){
			foreach(it,_dataList.l_exp001datas){
				Vector3d tmp;
				tmp(0) = it->pM.x;
				tmp(1) = it->pM.y;
				tmp(2) = it->pM.z;
				Y.push_back( tmp );
				mu_Y(0) += tmp(0);
				mu_Y(1) += tmp(1);
				mu_Y(2) += tmp(2);
			}
			mu_Y /= Np;
		} else if( typeid(T2) == typeid(mario::typeD) ) {
			foreach(it,_dataList.l_exp001datas){
				Vector3d tmp;
				tmp(0) = it->pD.x;
				tmp(1) = it->pD.y;
				tmp(2) = it->pD.z;
				Y.push_back( tmp );
				mu_Y(0) += tmp(0);
				mu_Y(1) += tmp(1);
				mu_Y(2) += tmp(2);
			}
			mu_Y /= Np;
		}
	}
	/* PとYの共分散行列S_PY */
	{
		S_PY = Matrix3d::Zero(3,3);
		times(i,0,Np){
			S_PY += ( P.at(i) - mu_P ) * ( Y.at(i) - mu_Y ).transpose();
		}
		S_PY /= Np;
	}
	S_PY_inv = S_PY.transpose();
	/* Qをつくる */
	{
		MatrixXd A(3,3);
		times(i,0,3){
			times(j,0,3){
				A(i,j) = S_PY(i,j) - S_PY_inv(i,j);
			}
		}
		MatrixXd Delta(3,1);
		MatrixXd Delta_trans(1,3);
		Delta(0) = A(2-1,3-1);
		Delta(1) = A(3-1,1-1);
		Delta(2) = A(1-1,2-1);
		Delta_trans = Delta.transpose();
		double trace = S_PY.trace();
		
		Matrix3d Q_rd = S_PY + S_PY_inv - trace * MatrixXd::Identity(3,3);
		// Qをつくる
		Matrix4d Q;
		Q(0,0) = trace;
		Q(0,1) = Delta_trans(0,0);
		Q(0,2) = Delta_trans(0,1);
		Q(0,3) = Delta_trans(0,2);
		Q(1,0) = Delta(0,0);
		Q(1,1) = Q_rd(0,0); 
		Q(1,2) = Q_rd(0,1);
		Q(1,3) = Q_rd(0,2);
		Q(2,0) = Delta(1,0);
		Q(2,1) = Q_rd(1,0);
		Q(2,2) = Q_rd(1,1);
		Q(2,3) = Q_rd(1,2);
		Q(3,0) = Delta(2,0);
		Q(3,1) = Q_rd(2,0);
		Q(3,2) = Q_rd(2,1);
		Q(3,3) = Q_rd(2,2);

		SelfAdjointEigenSolver<Matrix4d> es(Q);
		if( es.info() != Success ){
			abort();
		} else {
			int maxi = -1;
			double maxval = DBL_MIN;
			times(i,0,4){
				double val = es.eigenvalues()(i);
				if( val > maxval ){
					maxval = val;
					maxi = i;
				}
			}
			auto vec = es.eigenvectors();
			times(i,0,4){
				q_R(i) = vec(i,maxi);
			}
		}
	}

	return boost::shared_ptr<Eigen::MatrixXd>();
}

void mario::Experiment002::experimentLoop()
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
	::getTranslateMatrix<typeR,typeD>( datas );

}

string mario::Experiment002::inputFileNameLoop()
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

