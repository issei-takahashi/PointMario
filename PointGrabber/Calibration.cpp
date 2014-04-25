#include "Calibration.h"
#include "RedClusterDetecter.h"
#include "Display.h"
#include "Cross.h"
#include "Timer.h"
#include "Eventer.h"
#include "FileIO.h"

using namespace mario;

void Calibration::executeCalibration( list<Coordinate<typeD> > const & _calibPoints, string const & _filePath )
{
	Calibration::DataSet P('M'),Y('D');
	foreach(it,_calibPoints){
		cout << "+�}�[�N�Ɏ�������킹�Ă�������" << endl;

		auto disp = Display::getInstance();
		//disp->setScreenMode( true );
		auto cross = Cross::makeShared();
		cross->setDisplayPoint( *it );
		cross->displayStart();
		disp->moveActuatorTo(it->z);

		/* �G���^�[�L�[���������܂Ń��[�v */
		while( mario::Eventer::getInstance()->getIskeyPushed( mario::KeyType::KEY_RETURN ) == false ){
			auto ms1 = Timer::getInstance()->getms();
			disp->oneLoop();
			mario::Eventer::getInstance()->pollEvent();
			auto ms2 = Timer::getInstance()->getms();
			static int const FPS = FileIO::getConst("FPS");
			if( ms2 - ms1 < 1000.0/FPS ){
				disp->wait( 1000.0/FPS - ( ms2 - ms1 ) );
			}
		}
		disp->closeWindow();

		/* ����̈ʒu���v�� */
		Coordinate<typeM> jigCenter;
		while(1){
			jigCenter = Coordinate<typeM>(0,0,0);
			cout << "MeasureBasement�̏�������..." << endl;
			RedClusterDetecter base;
			base.start();
			static int const RED_CENTER_TIMES = FileIO::getConst("RED_CENTER_TIMES");
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
					jigCenter += thisred;
					cout << "*";
				}
			}
			cout << endl;
			jigCenter /= RED_CENTER_TIMES;
			base.stop();
			cout << RED_CENTER_TIMES <<"��v�����܂����D���ϒl��(" << jigCenter.x << "," << jigCenter.y << "," << jigCenter.z << ")�ł��D" << endl;
			cout << "��蒼���܂����H(y/n)�F";
			string againStr;
			cin  >> againStr;
			if( againStr == "y" ){
				continue;
			}else{
				break;
			}
		}
		// ���̎��_��jigCenter�Ɏ���̒��S(M���W)�������Ă���
		P.points.push_back( Eigen::Vector3d(jigCenter.x,jigCenter.y,jigCenter.z) );
		// cross->getDisplayPoint() �ɏ\�����S(D���W)�������Ă���
		auto p = cross->getDisplayPoint();
		Y.points.push_back( Eigen::Vector3d(p.x,p.y,p.z) );
	}
	boost::shared_ptr<Eigen::Matrix4d> affineMat;
	this->getAffineTransformation(P,Y,affineMat);
	FileIO::writeMatrix(_filePath,*affineMat);
}



/* ---------- Experiment002����R�s�y���Ă������� ---------- */


void Calibration::writeCalculatedValues(
	string const & _fileName,
	char type1, char type2,
	Calibration::DataSet const & P,
	Calibration::DataSet const & Y,
	Calibration::DataSet const & P_all,
	Calibration::DataSet const & Y_all,
	boost::shared_ptr<Eigen::Matrix4d> const& Affine,
	vector< boost::shared_ptr<Eigen::Vector3d> > const& Err,
	ofstream & tex_ofs )
{
	string filePath = string("calcdata/") + _fileName + "_" + type1 + type2 + ".csv";
	assert(P.points.size()==Y.points.size());
	assert(P_all.points.size()==Y_all.points.size());
	ofstream ofs( filePath, std::ios::out | std::ios::trunc );
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
	ofs << "//�A�t�B���ϊ�A" << endl;
	times(i,0,4){
		times(j,0,4){
			ofs << (*Affine)(i,j);
			if( j == 3 ){
				ofs << endl;
			} else {
				ofs << ",";
			}
		}
	}
	ofs << "//�덷" << endl;
	ofs <<type1<<"x,"<<type1<<"y,"<<type1<<"z,"<<type2<<"x,"<<type2<<"y,"<<type2<<"z,�덷x,�덷y,�덷z" << endl;
	double totalSquareErr = 0;
	double totalErrX = 0;
	double totalErrY = 0;
	double totalErrZ = 0;
	vector<double> errXVector;
	vector<double> errYVector;
	vector<double> errZVector;
	double totalScalarErr = 0;
	double totalScalarErrX = 0;
	double totalScalarErrY = 0;
	double totalScalarErrZ = 0;
	double totalScalarErrCenter = 0;
	vector<double> scalarErrVector;
	vector<double> scalarErrXVector;
	vector<double> scalarErrYVector;
	vector<double> scalarErrZVector;
	double maxScalarErr  = -99999;
	double maxScalarErrX = -99999;
	double maxScalarErrY = -99999;
	double maxScalarErrZ = -99999;
	auto itP_all = P_all.points.begin();
	auto itY_all = Y_all.points.begin();
	int i = 1;
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
		double tmpErrX = (*(*it))(0);
		double tmpErrY = (*(*it))(1);
		double tmpErrZ = (*(*it))(2);
		if( abs(tmpErrX) > maxScalarErrX ){
			maxScalarErrX = abs(tmpErrX);
		}
		if( abs(tmpErrY) > maxScalarErrY ){
			maxScalarErrY = abs(tmpErrY);
		}
		if( abs(tmpErrZ) > maxScalarErrZ ){
			maxScalarErrZ = abs(tmpErrZ);
		}
		totalErrX += tmpErrX;
		totalErrY += tmpErrY;
		totalErrZ += tmpErrZ;
		totalScalarErrX += abs( tmpErrX );
		totalScalarErrY += abs( tmpErrY );
		totalScalarErrZ += abs( tmpErrZ );
		totalSquareErr += (*it)->squaredNorm();
		double norm =  (*it)->norm();
		totalScalarErr += norm;
		if( norm > maxScalarErr ){
			maxScalarErr = norm;
		}
		errXVector.push_back(tmpErrX);
		errYVector.push_back(tmpErrY);
		errZVector.push_back(tmpErrZ);
		scalarErrVector.push_back(norm);
		scalarErrXVector.push_back(abs(tmpErrX));
		scalarErrYVector.push_back(abs(tmpErrY));
		scalarErrZVector.push_back(abs(tmpErrZ));
		// ���S�t�߂̓_
		int centerArr[27] = {32,33,34, 37,38,39, 42,43,44};
		times(ii,1,3){
			times(iii,0,9){
				centerArr[9*ii+iii] = centerArr[iii]+25*ii;
			}
		}
		for(int j=0;j<27;j++){
			if( centerArr[j] == i ){
				totalScalarErrCenter += norm;
			}
		}
		i++;
		itP_all++;
		itY_all++;
	}
	totalErrX /= Err.size();
	totalErrY /= Err.size();
	totalErrZ /= Err.size();
	totalScalarErrX /= Err.size();
	totalScalarErrY /= Err.size();
	totalScalarErrZ /= Err.size();
	totalScalarErrCenter /= 27;
	totalSquareErr /= Err.size();
	totalScalarErr /= Err.size();

	std::sort(errXVector.begin(),errXVector.end());
	std::sort(errYVector.begin(),errYVector.end());
	std::sort(errZVector.begin(),errZVector.end());
	std::sort(scalarErrVector.begin(),scalarErrVector.end());
	std::sort(scalarErrXVector.begin(),scalarErrXVector.end());
	std::sort(scalarErrYVector.begin(),scalarErrYVector.end());
	std::sort(scalarErrZVector.begin(),scalarErrZVector.end());

	tex_ofs << "\\shortstack{ " << _fileName << "\\\\ �} } &";
	/* xyz�S�����ɂ��� */
	ofs << "����x�N�g���̑傫���̕��ϒl," ;
	ofs << totalScalarErr << endl;
	tex_ofs << totalScalarErr << " & ";
	ofs << "����x�N�g���̑傫���̒����l," ;
	ofs << scalarErrVector.at( scalarErrVector.size()/2 ) << endl;
	//tex_ofs << scalarErrVector.at( scalarErrVector.size()/2 ) << " & ";
	ofs << "����x�N�g���̑傫���̍ő�l,";
	ofs << maxScalarErr << endl;
	tex_ofs << maxScalarErr << " & ";
	ofs << "����x�N�g���̑傫����2��̕��ϒl�i�ŏ������ꂽ���덷�j," ;
	ofs << totalSquareErr << endl;
	//tex_ofs << totalSquareErr << " & ";
	ofs << "���S�t��27�_�ɂ����邸��x�N�g���̑傫���̕��ϒl,";
	ofs << totalScalarErrCenter << endl;
	tex_ofs << totalScalarErrCenter << " & ";

	/* x,y,z�e�����ɂ��� */
	ofs << "x y z�̊e�����̂���i��������j�̕��ϒl,";
	ofs << totalErrX << "," << totalErrY << "," << totalErrZ << "," << endl;
	//tex_ofs << "\\shortstack{ x :  " << totalErrX << "\\\\y : " << totalErrY << "\\\\ z : " << totalErrZ << "} & " ;
	ofs << "x y z�̊e�����̂���i��������j�̒����l,";
	ofs << errXVector.at( errXVector.size()/2 ) << "," << errYVector.at( errYVector.size()/2 ) << "," << errZVector.at( errZVector.size()/2 ) << "," << endl;
	//tex_ofs << "\\shortstack{ x :  " << errXVector.at( errXVector.size()/2 ) << "\\\\y : " << errYVector.at( errYVector.size()/2 ) << "\\\\ z : " << errZVector.at( errZVector.size()/2 ) << "} & ";
	ofs << "x y z�̊e�����̂���̑傫���̕��ϒl,";
	ofs << totalScalarErrX << "," << totalScalarErrY << "," << totalScalarErrZ << "," << endl;
	tex_ofs << "\\shortstack{ x :  " << totalScalarErrX << "\\\\y : " << totalScalarErrY << "\\\\z : " << totalScalarErrZ << "} ";
	ofs << "x y z�̊e�����̂���̑傫���̒����l,";
	ofs << scalarErrXVector.at( scalarErrXVector.size()/2 ) << "," << scalarErrYVector.at( scalarErrYVector.size()/2 ) << "," << scalarErrZVector.at( scalarErrZVector.size()/2 ) << "," << endl;
	//tex_ofs << "\\shortstack{ x :  " << scalarErrXVector.at( scalarErrXVector.size()/2 ) << "\\\\y : " << scalarErrYVector.at( scalarErrYVector.size()/2 ) << "\\\\ z : " << scalarErrZVector.at( scalarErrZVector.size()/2 ) << "} & ";
	ofs << "x y z�̊e�����̂���̑傫���̍ő�l,";
	ofs << maxScalarErrX << "," << maxScalarErrY << "," << maxScalarErrZ << "," << endl;
	//tex_ofs << "\\shortstack{ x :  " << maxScalarErrX << "\\\\y : " << maxScalarErrY << "\\\\ z : " << maxScalarErrZ << "} " ;
	
	tex_ofs << "\\\\  \\hline" << endl;
	/* �e�������̌덷���x�N�g���v���b�g�ƃX�J���[�v���b�g�`���� */
	times(j,0,3){
		static char xyz[3] = {'x','y','z'};

		//string plotFilePathS = string("plotdata/") + type1 + type2 + _fileName + xyz[j] + "_s" + ".csv";
		//ofstream tmpofsS( plotFilePathS, std::ios::out | std::ios::trunc );
		//tmpofsS << "�f�[�^�`��,3" << endl;
		//tmpofsS << type1 << "����" << type2 << "�̕ϊ��ŁC" << type2 << "����125�_�ɂ�����" << endl;
		//tmpofsS << xyz[j] << "�����̌덷�̑傫���ł��D" << endl;

		string plotFilePathV = string("plotdata/") + type1 + type2 + _fileName + xyz[j] + ".csv";
		ofstream tmpofsV( plotFilePathV, std::ios::out | std::ios::trunc );
		tmpofsV << "�f�[�^�`��,5" << endl;
		tmpofsV << type1 << "����" << type2 << "�̕ϊ��ŁC" << type2 << "����125�_�ɂ�����" << endl;
		tmpofsV << xyz[j] << "�����̂���x�N�g���ł��D" << endl;

		auto itY_all = Y_all.points.begin();
		foreach(it,Err){
			///* Scalar */
			//// ->Y
			//tmpofsS << (*itY_all)(0) << ",";
			//tmpofsS << (*itY_all)(1) << ",";
			//tmpofsS << (*itY_all)(2) << ",";
			//// P->Y�̌덷(j=0(x),1(y),2(z))
			//tmpofsS << abs((*(*it))(j)) << endl;
			/* Vector */
			// ->Y
			tmpofsV << (*itY_all)(0) << ",";
			tmpofsV << (*itY_all)(1) << ",";
			tmpofsV << (*itY_all)(2) << ",";
			// P->Y�̌덷(j=0(x),1(y),2(z))
			times(k,0,3){
				if( k == j ){
					tmpofsV << (*(*it))(j) ;
				} else {
					tmpofsV << "0" ;
				}
				if( k == 2 ){
					tmpofsV << endl;
				} else {
					tmpofsV << ",";
				}
			}
			itY_all++;
		}
	}
	/* �S�������̌덷���x�N�g���v���b�g�ƃX�J���[�v���b�g�`���� */
	{
		//string plotFilePathS = string("plotdata/") + _fileName + "_" + type1 + type2 + "xyz" + "_scalar" + ".csv";
		//ofstream tmpofsS( plotFilePathS, std::ios::out | std::ios::trunc );
		//tmpofsS << "�f�[�^�`��,3" << endl;
		//tmpofsS << type1 << "����" << type2 << "�̕ϊ��ŁC" << type2 << "����125�_�ɂ�����" << endl;
		//tmpofsS << "�덷�̑傫���ł��D" << endl;

		string plotFilePathV = string("plotdata/") + type1 + type2 + _fileName + ".csv";
		ofstream tmpofsV( plotFilePathV, std::ios::out | std::ios::trunc );
		tmpofsV << "�f�[�^�`��,5" << endl;
		tmpofsV << type1 << "����" << type2 << "�̕ϊ��ŁC" << type2 << "����125�_�ɂ�����" << endl;
		tmpofsV << "����̃x�N�g���ł��D" << endl;

		auto itY_all = Y_all.points.begin();
		foreach(it,Err){
			///* Scalar */
			//// ->Y
			//tmpofsS << (*itY_all)(0) << ",";
			//tmpofsS << (*itY_all)(1) << ",";
			//tmpofsS << (*itY_all)(2) << ",";
			//// P->Y�̌덷(j=0(x),1(y),2(z))
			//tmpofsS << (*(*it)).norm() << endl;
			/* Vector */
			// ->Y
			tmpofsV << (*itY_all)(0) << ",";
			tmpofsV << (*itY_all)(1) << ",";
			tmpofsV << (*itY_all)(2) << ",";
			// P->Y�̌덷(j=0(x),1(y),2(z))
			tmpofsV << (*(*it))(0) << ",";
			tmpofsV << (*(*it))(1) << ",";
			tmpofsV << (*(*it))(2) << endl;
			itY_all++;
		}
	}
}

// �_�QP->�_�QY �ւ̍��̉^���ϊ������߂�
void Calibration::getTranslateMatrix( 
	Calibration::DataSet const & P,
	Calibration::DataSet const & Y,
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

// �_�QP->�_�QY �ւ̃A�t�B���ϊ������߂�
void Calibration::getAffineTransformation( 
	Calibration::DataSet const & P,
	Calibration::DataSet const & Y,
	boost::shared_ptr<Eigen::Matrix4d>& _affineDst )
{
	using namespace Eigen;
	assert( P.points.size() == Y.points.size() );
	int const N = P.points.size();
	/*

	xi' = [ a d g tx ] [xi]
	yi' = [ b e h ty ] [yi]
	zi' = [ c f j tz ] [zi]
	1  =  [ 0 0 0  1 ] [1]
	(i=1,2,...,N)

	[ x1    y1     z1    1     ] [a ]   [x1']
	[   x1    y1     z1    1   ] [b ]   [y1']
	[     x1    y1     z1    1 ] [c ]   [z1']
	[ x2    y2     z2    1     ] [d ]   [x2']
	[   x2    y2     z2    1   ] [e ]   [y2']
	[     x2    y2     z2    1 ] [f ] = [z2']
	.               [g ]   [.  ]
	.               [h ]   [.  ]
	.               [i ]   [.  ]
	[ xN    yN     zN    1     ] [tx]   [xN']
	[   xN    yN     zN    1   ] [ty]   [yN']
	[     xN    yN     zN    1 ] [tz]   [zN']

	*/
	MatrixXd A = MatrixXd::Zero(N*3,12);
	VectorXd V    = VectorXd::Zero(N*3);
	times(i,0,N){
		V(i*3+0)     = Y.points.at(i)(0); // xi' 
		V(i*3+1)     = Y.points.at(i)(1); // yi'
		V(i*3+2)     = Y.points.at(i)(2); // zi'

		times(k,0,3){
			A(i*3+k,0+k) = P.points.at(i)(0); // xi
			A(i*3+k,3+k) = P.points.at(i)(1); // yi
			A(i*3+k,6+k) = P.points.at(i)(2); // zi
			A(i*3+k,9+k) = 1;
		}
	}

	//cout << "Its singular values are:" << endl << svd.singularValues() << endl;
	//cout << "Its left singular vectors are the columns of the thin U matrix:" << endl << svd.matrixU() << endl;
	//cout << "Its right singular vectors are the columns of the thin V matrix:" << endl << svd.matrixV() << endl;
	//cout << "Now consider this rhs vector:" << endl << V << endl;
	//cout << ":" << endl << svd.solve(V) << endl;

	cout << "SVD�@�ŃA�t�B���ϊ����v�Z���c" << endl;
	//JacobiSVD<MatrixXd> svd( A, ComputeThinU | ComputeThinV );
	//cout << svd.solve(V) << endl;
	//cout << solved << endl;

	VectorXd solved(12);
	A.jacobiSvd(ComputeThinU | ComputeThinV).solve(V).evalTo(solved); 

	_affineDst = (boost::shared_ptr<Matrix4d>)( new Matrix4d() );
	(*_affineDst)(0,0) = solved(0);  (*_affineDst)(0,1) = solved(3);  (*_affineDst)(0,2) = solved(6);  (*_affineDst)(0,3) = solved(9);
	(*_affineDst)(1,0) = solved(1);  (*_affineDst)(1,1) = solved(4);  (*_affineDst)(1,2) = solved(7);  (*_affineDst)(1,3) = solved(10);
	(*_affineDst)(2,0) = solved(2);  (*_affineDst)(2,1) = solved(5);  (*_affineDst)(2,2) = solved(8);  (*_affineDst)(2,3) = solved(11);
	(*_affineDst)(3,0) = 0;          (*_affineDst)(3,1) = 0;          (*_affineDst)(3,2) = 0;          (*_affineDst)(3,3) = 1;

}


// �ϊ��̂�������߂�
void Calibration::getErrors( 
	Calibration::DataSet const & P,
	Calibration::DataSet const & Y,
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
		(*err)(0) = tmp(0) - (*itY)(0);
		(*err)(1) = tmp(1) - (*itY)(1);
		(*err)(2) = tmp(2) - (*itY)(2);
		Err.push_back(err);
		itP++;
		i++;
	}
	_errDst = Err;
}

// �ϊ��̂�������߂�
void Calibration::getErrors( 
	Calibration::DataSet const & P,
	Calibration::DataSet const & Y,
	boost::shared_ptr<Eigen::Matrix4d> const& Affine,
	vector< boost::shared_ptr<Eigen::Vector3d> > & _errDst )
{
	using namespace Eigen;
	vector< boost::shared_ptr<Vector3d> > Err; // Y-Y'
	auto itP = P.points.begin();
	int i = 0;
	foreach(itY,Y.points){
		Vector4d tmpv;
		tmpv(0) = (*itP)(0);
		tmpv(1) = (*itP)(1);
		tmpv(2) = (*itP)(2);
		tmpv(3) = 1.0;
		Vector4d tmp = (*Affine) * tmpv;
		boost::shared_ptr<Vector3d> err( new Vector3d() );
		(*err)(0) = tmp(0) - (*itY)(0);
		(*err)(1) = tmp(1) - (*itY)(1);
		(*err)(2) = tmp(2) - (*itY)(2);
		Err.push_back(err);
		itP++;
		i++;
	}
	_errDst = Err;
}


