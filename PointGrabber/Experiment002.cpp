#include "Experiment.h"
#include "ExperimentData.h"
#include "utils.h"

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
			char ptype = this->inputCoordinateTypeLoop( "�ϊ���(RorMorD)����͂��Ă��������F" );
			char ytype = this->inputCoordinateTypeLoop( "�ϊ���(RorMorD)����͂��Ă��������F" );
			DataSet P(ptype),Y(ytype);
			boost::shared_ptr<Eigen::Matrix3d> R;
			boost::shared_ptr<Eigen::Vector3d> q_T;
			{
				cout << "�S�Ă̓_���g���܂����H(y/n)�F";
				string str;
				cin >> str;
				if( str == "y" ){
					this->makeDataSet( datas, P, Y, true );
				} else {
					this->makeDataSet( datas, P, Y, false );
				}	
			}
			this->getTranslateMatrix( P, Y, R, q_T );
			vector< boost::shared_ptr<Eigen::Vector3d> > Err;
			DataSet P_all(ptype),Y_all(ytype);
			this->makeDataSet( datas, P_all, Y_all, true );
			this->getErrors( P_all, Y_all, R, q_T, Err );
			cout << "-----��]����R-----" << endl;
			cout << *R << endl;
			cout << "-----���i����q_T-----" << endl;
			cout << *q_T << endl;
			cout << "------------------" << endl;
			cout << "��蒼���܂����H(y/n)�F";
			string againStr;
			cin >> againStr;
			if( againStr == "y" ){
				continue;
			}
			string saveFileName;
			saveFileName = this->inputFileNameLoop("calcdata/���ɕۑ�����t�@�C��������͂��Ă��������F");
			this->writeCalculatedValues(string("calcdata/")+saveFileName+".csv",P.type,Y.type,P,Y,P_all,Y_all,R,q_T,Err);
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
	mario::Experiment002::DataSet const & P,
	mario::Experiment002::DataSet const & Y,
	mario::Experiment002::DataSet const & P_all,
	mario::Experiment002::DataSet const & Y_all,
	boost::shared_ptr<Eigen::Matrix3d> const& R,
	boost::shared_ptr<Eigen::Vector3d> const& q_T,
	vector< boost::shared_ptr<Eigen::Vector3d> > const& Err )
{
	assert(P.points.size()==Y.points.size());
	assert(P_all.points.size()==Y_all.points.size());
	ofstream ofs( _filePath, std::ios::out | std::ios::trunc );
	ofs << "//" << type1 << "->" << type2 << "�̍��W�ϊ�" << endl;
	ofs << "//" << type1 << "�̂����g�����_" << endl;
	ofs << "x,y,z" << endl;
	foreach(it,P.points){
		ofs << (*it)(0) << "," << (*it)(1) << "," << (*it)(2) << endl;
	}
	ofs << "//" << type2 << "�̂����g�����_" << endl;
	ofs << "x,y,z" << endl;
	foreach(it,Y.points){
		ofs << (*it)(0) << "," << (*it)(1) << "," << (*it)(2) << endl;
	}	
	ofs << "//��]R" << endl;
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
	ofs << "//���iq_T" << endl;
	ofs << "x,y,z" << endl;
	times(i,0,3){
		ofs << (*q_T)(i);
		if( i == 2 ){
			ofs << endl;
		} else {
			ofs << ",";
		}
	}
	ofs << "//�덷" << endl;
	ofs <<type1<<"x,"<<type1<<"y,"<<type1<<"z,"<<type2<<"x,"<<type2<<"y,"<<type2<<"z,�덷x,�덷y,�덷z" << endl;
	int i = 1;
	double totalErr = 0;
	auto itP_all = P_all.points.begin();
	auto itY_all = Y_all.points.begin();
	foreach(it,Err){
		// P->
		ofs << (*itP_all)(0) << ",";
		ofs << (*itP_all)(1) << ",";
		ofs << (*itP_all)(2) << ",";
		// ->Y
		ofs << (*itY_all)(0) << ",";
		ofs << (*itY_all)(1) << ",";
		ofs << (*itY_all)(2) << ",";
		// P->Y�̌덷
		ofs << (*(*it))(0) << ",";
		ofs << (*(*it))(1) << ",";
		ofs << (*(*it))(2) << "," << endl;
		totalErr += (*it)->squaredNorm();
		i++;
		itP_all++;
		itY_all++;
	}
	totalErr /= Err.size();
	ofs << "//�ŏ������ꂽ2��덷" << endl;
	ofs << totalErr << endl;
}


void mario::Experiment002::makeDataSet( mario::Experiment001DataList const & _dataList, DataSet & P, DataSet & Y, bool _allFlag )
{
	using namespace Eigen;
	/* �g���_��I�� */
	map<int,int> indexes;
	if( _allFlag ){
		times(i,0,_dataList.l_exp001datas.size()){
			indexes.insert( make_pair(i,i) );
		}
		
	} else {
		vector<Vector3d> points;
		int count = 0;
		while(1){
			int x=-1,y=-1,z=-1;
			int thisIndex=-1;
			while(1){
				string line;
				vector<string> vline;
				cout << "�v�Z�Ɏg���_��R���W(mm)����͂��Ă��������F";
				cin >> line;
				utils::cutLine(line,vline);
				if( vline.size()==3 && utils::isNumber(vline.at(0)) && utils::isNumber(vline.at(1)) && utils::isNumber(vline.at(2)) ){
					x = utils::string2int(vline.at(0));
					y = utils::string2int(vline.at(1));
					z = utils::string2int(vline.at(2));
					if( x%50==0 && 0<=x && x<=200 &&
						y%50==0 && 0<=y && y<=200 &&
						z%50==0 && 0<=z && z<=200  ){
							int tmpIndex = 5*(x/50) + 25*(y/50) + (z/50);
							if( indexes.find(tmpIndex) == indexes.end() ){
								thisIndex = tmpIndex;
								break;
							}
							else{
								cout <<"("<<x<<","<<y<<","<<z<<")�͊��ɓo�^����Ă��܂��D" << endl;
							}
					}
					else{
						cout << "50�̔{������0�`200�̒l�œ��͂��Ă��������D" << endl;
					}
				}
			}
			Vector3d tmp;
			tmp(0)=x; tmp(1)=y; tmp(2)=z;
			count++;
			points.push_back( tmp );
			indexes.insert( make_pair(thisIndex,thisIndex) );
			// �\�[�g�p�t�@���N�^
			struct point_compare
			{
				bool operator()(const Vector3d& lhs, const Vector3d& rhs) const {
					return 5*(lhs(0)/50)+25*(lhs(1)/50)+(lhs(2)/50) < 5*(rhs(0)/50)+25*(rhs(1)/50)+(rhs(2)/50);
				}
			};
			std::sort(points.begin(),points.end(),point_compare());
			cout << "����" << count << "�_" << endl;  
			foreach(it,points){
				cout << "(" << (*it)(0) << "," << (*it)(1) << "," << (*it)(2) << ") ";
			}
			cout << endl;
			cout << "�����Ŋm�肵�܂����H(y/n)�F";
			string okStr;
			cin >> okStr;
			if( okStr == "y" ){
				break;
			}
			else{
				continue;
			}
		}
	}
	/* �_�Q�����������āC���łɏd�S�Z�o */
	// P
	if( P.type == 'R' ){
		int count = 0;
		foreach(it,_dataList.l_exp001datas){
			if( indexes.find(count++) != indexes.end() ){
				Vector3d tmp;
				tmp(0) = it->pR.x;
				tmp(1) = it->pR.y;
				tmp(2) = it->pR.z;
				P.points.push_back( tmp );
				P.average(0) += tmp(0);
				P.average(1) += tmp(1);
				P.average(2) += tmp(2);
			}
		}
		P.average /= P.points.size();
	} else if(  P.type == 'M' ) {
		int count = 0;
		foreach(it,_dataList.l_exp001datas){
			if( indexes.find(count++) != indexes.end() ){
				Vector3d tmp;
				tmp(0) = it->pM.x;
				tmp(1) = it->pM.y;
				tmp(2) = it->pM.z;
				P.points.push_back( tmp );
				P.average(0) += tmp(0);
				P.average(1) += tmp(1);
				P.average(2) += tmp(2);
			}
		}
		P.average /= P.points.size();
	} else if(  P.type == 'D' ) {
		int count = 0;
		foreach(it,_dataList.l_exp001datas){
			if( indexes.find(count++) != indexes.end() ){
				Vector3d tmp;
				tmp(0) = it->pD.x;
				tmp(1) = it->pD.y;
				tmp(2) = it->pD.z;
				P.points.push_back( tmp );
				P.average(0) += tmp(0);
				P.average(1) += tmp(1);
				P.average(2) += tmp(2);
			}
		}
		P.average /= P.points.size();
	}

	// Y
	if( Y.type == 'R' ) {
		int count = 0;
		foreach(it,_dataList.l_exp001datas){
			if( indexes.find(count++) != indexes.end() ){
				Vector3d tmp;
				tmp(0) = it->pR.x;
				tmp(1) = it->pR.y;
				tmp(2) = it->pR.z;
				Y.points.push_back( tmp );
				Y.average(0) += tmp(0);
				Y.average(1) += tmp(1);
				Y.average(2) += tmp(2);
			}
		}
		Y.average /= Y.points.size();
	} else if( Y.type == 'M' ){
		int count = 0;
		foreach(it,_dataList.l_exp001datas){
			if( indexes.find(count++) != indexes.end() ){
				Vector3d tmp;
				tmp(0) = it->pM.x;
				tmp(1) = it->pM.y;
				tmp(2) = it->pM.z;
				Y.points.push_back( tmp );
				Y.average(0) += tmp(0);
				Y.average(1) += tmp(1);
				Y.average(2) += tmp(2);
			}
		}
		Y.average /= Y.points.size();
	} else if( Y.type == 'D' ) {
		int count = 0;
		foreach(it,_dataList.l_exp001datas){
			if( indexes.find(count++) != indexes.end() ){
				Vector3d tmp;
				tmp(0) = it->pD.x;
				tmp(1) = it->pD.y;
				tmp(2) = it->pD.z;
				Y.points.push_back( tmp );
				Y.average(0) += tmp(0);
				Y.average(1) += tmp(1);
				Y.average(2) += tmp(2);
			}
		}
		Y.average /= Y.points.size();
	}
}

// �_�QP->�_�QY �ւ̊􉽕ϊ������߂�
void mario::Experiment002::getTranslateMatrix( 
	mario::Experiment002::DataSet const & P, mario::Experiment002::DataSet const & Y,
	boost::shared_ptr<Eigen::Matrix3d>& _rotDst,
	boost::shared_ptr<Eigen::Vector3d>& _transDst )
{
	assert(P.points.size()==Y.points.size());
	int const Np = P.points.size();
	using namespace Eigen;
	Vector3d mu_P = P.average; // P�̏d�S
	Vector3d mu_Y = Y.average; // Y�̏d�S
	Matrix3d S_PY = Matrix3d::Zero(); // P��Y�̋����U�s��
	Matrix3d S_PY_trans = Matrix3d::Zero(); // S_PY�̓]�u
	Vector4d q_R = Vector4d::Zero();

	/* P��Y�̋����U�s��S_PY */
	{
		S_PY = Matrix3d::Zero(3,3);
		times(i,0,Np){
			S_PY += ( P.points.at(i) - mu_P ) * ( Y.points.at(i) - mu_Y ).transpose();
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
			double maxval = -DBL_MAX;
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

	_rotDst = R;
	_transDst = q_T;
}

void mario::Experiment002::getErrors( 
	mario::Experiment002::DataSet const & P,
	mario::Experiment002::DataSet const & Y,
	boost::shared_ptr<Eigen::Matrix3d> const& R,
	boost::shared_ptr<Eigen::Vector3d> const& q_T,
	vector< boost::shared_ptr<Eigen::Vector3d> > & _errDst )
{
	using namespace Eigen;
	vector< boost::shared_ptr<Vector3d> > Err; // Y-Y'
	auto itP = P.points.begin();
	int i = 0;
	foreach(itY,Y.points){
		Vector3d tmp = (*R)*(*itP) + *q_T;
		boost::shared_ptr<Vector3d> err( new Vector3d() );
		(*err)(0) = (*itY)(0) - tmp(0);
		(*err)(1) = (*itY)(1) - tmp(1);
		(*err)(2) = (*itY)(2) - tmp(2);
		Err.push_back(err);
		itP++;
		i++;
	}
	_errDst = Err;
}