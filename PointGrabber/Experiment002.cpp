#include "Experiment.h"
#include "ExperimentData.h"

// �_�QP->�_�QY �ւ̊􉽕ϊ������߂�

void getTranslateMatrix( char _type1, char _type2, mario::Experiment001DataList const & _dataList,
	boost::shared_ptr<Eigen::Matrix3d>& _rotDst,
	boost::shared_ptr<Eigen::Vector3d>& _transDst,
	vector< boost::shared_ptr<Eigen::Vector3d> >& _errDst )
{
	int const Np = _dataList.l_exp001datas.size();
	using namespace Eigen;
	vector<Vector3d> P; // 3�����_�QP
	vector<Vector3d> Y; // 3�����_�QY
	vector< boost::shared_ptr<Vector3d> > Err; // Y-Y'
	Vector3d mu_P = Vector3d::Zero(); // P�̏d�S
	Vector3d mu_Y = Vector3d::Zero(); // Y�̏d�S
	Matrix3d S_PY = Matrix3d::Zero(); // P��Y�̋����U�s��
	Matrix3d S_PY_trans = Matrix3d::Zero(); // S_PY�̓]�u
	Vector4d q_R = Vector4d::Zero();

	/* �_�Q�����������āC���łɏd�S�Z�o */
	{
		// P
		if( _type1 == 'R' ){
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
		} else if( _type1 == 'M' ) {
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
		} else if( _type1 == 'D' ) {
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
		if( _type2 == 'R' ) {
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
		} else if( _type2 == 'M' ){
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
		} else if( _type2 == 'D' ) {
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
	/* P��Y�̋����U�s��S_PY */
	{
		S_PY = Matrix3d::Zero(3,3);
		times(i,0,Np){
			S_PY += ( P.at(i) - mu_P ) * ( Y.at(i) - mu_Y ).transpose();
		}
		S_PY /= Np;
	}
	S_PY_trans = S_PY.transpose();

	/* q_R������ */
	{
		MatrixXd A(3,3);
		times(i,0,3){
			times(j,0,3){
				A(i,j) = S_PY(i,j) - S_PY_trans(i,j);
			}
		}
		MatrixXd Delta(3,1);
		MatrixXd Delta_trans(1,3);
		Delta(0) = A(2-1,3-1);
		Delta(1) = A(3-1,1-1);
		Delta(2) = A(1-1,2-1);
		Delta_trans = Delta.transpose();
		double trace = S_PY.trace();

		Matrix3d Q_rd = S_PY + S_PY_trans - trace * MatrixXd::Identity(3,3);
		// Q������
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
				double val = es.eigenvalues()(i,0);
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

	/* ��]�s��R(q_R)�ƕ��i�x�N�g��q_T����� */
	boost::shared_ptr<Matrix3d> R( new Matrix3d() );
	boost::shared_ptr<Vector3d> q_T( new Vector3d() );
	{
		double q0,q1,q2,q3;
		q0 = q_R(0);
		q1 = q_R(1);
		q2 = q_R(2);
		q3 = q_R(3);
		(*R)(0,0) = q0*q0 +q1*q1 -q2*q2 -q3*q3;  (*R)(0,1) = 2*(q1*q2 -q0*q3);               (*R)(0,2) = 2*(q1*q3 +q0*q2);
		(*R)(1,0) = 2*(q1*q2 +q0*q3);            (*R)(1,1) = q0*q0 -q1*q1 +q2*q2 -q3*q3;     (*R)(1,2) = 2*(q2*q3 -q0*q1);
		(*R)(2,0) = 2*(q1*q3 -q0*q2);            (*R)(2,1) = 2*(q2*q3 +q0*q1);               (*R)(2,2) = q0*q0 -q1*q1 -q2*q2 +q3*q3;
	}
	*q_T = mu_Y - (*R)*mu_P;
	auto itP = P.begin();
	int i = 0;
	foreach(itY,Y){
		Vector3d tmp = (*R)*(*itP) + *q_T;
		boost::shared_ptr<Vector3d> err( new Vector3d() );
		(*err)(0) = (*itY)(0) - tmp(0);
		(*err)(1) = (*itY)(1) - tmp(1);
		(*err)(2) = (*itY)(2) - tmp(2);
		Err.push_back(err);
		itP++;
		i++;
	}
	_rotDst = R;
	_transDst = q_T;
	_errDst = Err;
}

void mario::Experiment002::experimentLoop()
{
	while(1){
		Experiment001DataList datas;
		while(1){
			string filePath = string("exdata/") + this->inputFileNameLoop("exdata/������ǂݍ���xml�t�@�C����(�g���q����)����͂��Ă��������F") + ".xml";
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
		while(1){
			char type1 = this->inputCoordinateTypeLoop( "�ϊ���(RorMorD)����͂��Ă��������F" );
			char type2 = this->inputCoordinateTypeLoop( "�ϊ���(RorMorD)����͂��Ă��������F" );
			boost::shared_ptr<Eigen::Matrix3d> R;
			boost::shared_ptr<Eigen::Vector3d> q_T;
			vector< boost::shared_ptr<Eigen::Vector3d> > Err;
			::getTranslateMatrix( type1, type2, datas, R, q_T, Err );
			cout << "-----��]����R-----" << endl;
			cout << *R << endl;
			cout << "-----���i����q_T-----" << endl;
			cout << *q_T << endl;
			cout << "------------------" << endl;
			string saveFileName;
			saveFileName = this->inputFileNameLoop("�ϊ�R�����q_T�ƌ덷�����L�^����t�@�C��������͂��Ă��������F");
			this->writeCalculatedValues(saveFileName+".csv",type1,type2,R,q_T,Err);
			cout << saveFileName << ".csv�ɕۑ����܂����D" << endl;
			cout << "�I�����܂����H(y/n)�F";
			string okStr;
			cin  >> okStr;
			if( okStr == "y" ){
				break;
			}
		}
	}
}

string mario::Experiment002::inputFileNameLoop( string const & _message )
{
	string buf = "";
	bool okFlag = false;
	while( okFlag == false ){
		cout << _message;
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

char mario::Experiment002::inputCoordinateTypeLoop( string const & _message )
{
	string buf = "";
	bool okFlag = false;
	while( okFlag == false ){
		cout << _message;
		cin  >> buf;
		if( buf == "R" || buf == "M" || buf == "D" ){
			okFlag = true;
		}
	}
	return buf.at(0);
}

void mario::Experiment002::writeCalculatedValues(
	string const & _filePath,
	char type1, char type2,
	boost::shared_ptr<Eigen::Matrix3d> const& R,
	boost::shared_ptr<Eigen::Vector3d> const& q_T,
	vector< boost::shared_ptr<Eigen::Vector3d> > const& Err )
{
	ofstream ofs( _filePath, std::ios::out | std::ios::trunc );
	ofs << type1 << "->" << type2 << "�̍��W�ϊ�" << endl;
	ofs << "//��]" << endl;
	times(i,0,3){
		times(j,0,3){
			ofs << (*R)(i,j);
			if( j == 2 ){
				ofs << endl;
			} else {
				ofs << ",";
			}
		}
	}
	ofs << "//���i" << endl;
	times(i,0,3){
		ofs << (*q_T)(i);
		if( i == 2 ){
			ofs << endl;
		} else {
			ofs << ",";
		}
	}
	ofs << "//�덷" << endl;
	int i = 1;
	foreach(it,Err){
		ofs << i << ",";
		ofs << (*(*it))(0) << ",";
		ofs << (*(*it))(1) << ",";
		ofs << (*(*it))(2) << "," << endl;
		i++;
	}
}
