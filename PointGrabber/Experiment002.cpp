#include "Experiment.h"
#include "ExperimentData.h"
#include "utils.h"

void mario::Experiment002::experimentLoop()
{
	while(1){
		Experiment001DataList datas;
		while(1){
			string filePath = string("exdata/") + this->inputFileNameLoop("exdata/内から読み込むxmlファイル名(拡張子抜き)を入力してください：") + ".xml";
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
		vector<string> files;
		{
			ifstream ifs( "data/calibset/file_names.csv" );
			string line;
			vector<string> vline;
			while( ifs && getline( ifs, line ) ){
				utils::cutLine( line, vline );
				files.push_back( vline.at(0) );
			}
		}
		char types[3] = {'R','M','D'};
		times(j,0,3){
			times(k,0,3){
				if( j != k ){
					char ptype = types[j]; 
					char ytype = types[k];
					if( ( ptype == 'M' && ytype == 'D' ) ||
						( ptype == 'D' && ytype == 'R' ) ||
						( ptype == 'M' && ytype == 'R' ) ){
						string texPath = string("calcdata/tex_") + ptype + ytype + ".txt";
						ofstream tex_ofs( texPath, std::ios::out | std::ios::app | std::ios::ate );
						tex_ofs << "名前 & ずれベクトルのノルムの平均値 & " ;
						//tex_ofs << "ずれベクトルのノルムの中央値 & " ;
						tex_ofs << "ずれベクトルのノルムの最大値 & ";
						//tex_ofs << "ずれベクトルのノルムの2乗の平均値（最小化された二乗誤差） & " ;
						tex_ofs << "x y zの各方向のずれ（符号あり）の平均値 & ";
						//tex_ofs << "x y zの各方向のずれ（符号あり）の中央値 & ";
						tex_ofs << "x y zの各方向のずれのノルムの平均値 & ";
						//tex_ofs << "x y zの各方向のずれのノルムの中央値 & ";
						tex_ofs << "x y zの各方向のずれのノルムの最大値";
						tex_ofs << "\\\\  \\hline" << endl;
						times(i,0,files.size()){
							DataSet P(ptype),Y(ytype);
							boost::shared_ptr<Eigen::Matrix4d> Affine;
							this->makeDataSetFromCsv( string("data/calibset/")+files[i]+".csv", datas, P, Y );	
							this->getAffineTransformation( P, Y, Affine );
							vector< boost::shared_ptr<Eigen::Vector3d> > Err;
							DataSet P_all(ptype),Y_all(ytype);
							this->makeDataSetFromCsv( "data/calibset/all.csv", datas, P_all, Y_all );
							this->getErrors( P_all, Y_all, Affine, Err );
							cout << "-----アフィン変換A-----" << endl;
							cout << *Affine << endl;
							cout << "------------------" << endl;
							this->writeMatrix( files[i], ptype, ytype, Affine );
							//this->writeGraphWithGL( "", Y_all, Err );
						}
					}
				}
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
		cout << buf << " でよろしいですか？(y/n)：";
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

void mario::Experiment002::writeMatrix( string const & _fileName,
	char type1, char type2,
	boost::shared_ptr<Eigen::Matrix4d> const& Affine )
{
	string filePath = string("calcdata/") + _fileName + "_" + type1 + type2 + ".csv";
	ofstream ofs( filePath, std::ios::out | std::ios::trunc );
	ofs << "//" << type1 << "->" << type2 << "の座標変換" << endl;

	ofs << "//アフィン変換A" << endl;
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
}

void mario::Experiment002::writeCalculatedValues(
	string const & _fileName,
	char type1, char type2,
	mario::Experiment002::DataSet const & P,
	mario::Experiment002::DataSet const & Y,
	mario::Experiment002::DataSet const & P_all,
	mario::Experiment002::DataSet const & Y_all,
	boost::shared_ptr<Eigen::Matrix4d> const& Affine,
	vector< boost::shared_ptr<Eigen::Vector3d> > const& Err,
	ofstream & tex_ofs )
{
	string filePath = string("calcdata/") + _fileName + "_" + type1 + type2 + ".csv";
	assert(P.points.size()==Y.points.size());
	assert(P_all.points.size()==Y_all.points.size());
	ofstream ofs( filePath, std::ios::out | std::ios::trunc );
	ofs << "//" << type1 << "->" << type2 << "の座標変換" << endl;
	ofs << "//" << type1 << "のうち使った点" << endl;
	ofs << "x,y,z" << endl;
	foreach(it,P.points){
		ofs << (*it)(0) << "," << (*it)(1) << "," << (*it)(2) << endl;
	}
	ofs << "//" << type2 << "のうち使った点" << endl;
	ofs << "x,y,z" << endl;
	foreach(it,Y.points){
		ofs << (*it)(0) << "," << (*it)(1) << "," << (*it)(2) << endl;
	}	
	ofs << "//アフィン変換A" << endl;
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
	ofs << "//誤差" << endl;
	ofs <<type1<<"x,"<<type1<<"y,"<<type1<<"z,"<<type2<<"x,"<<type2<<"y,"<<type2<<"z,誤差x,誤差y,誤差z" << endl;
	int i = 1;
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
	foreach(it,Err){
		// P->
		ofs << (*itP_all)(0) << ",";
		ofs << (*itP_all)(1) << ",";
		ofs << (*itP_all)(2) << ",";
		// ->Y
		ofs << (*itY_all)(0) << ",";
		ofs << (*itY_all)(1) << ",";
		ofs << (*itY_all)(2) << ",";
		// P->Yの誤差
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
	totalSquareErr /= Err.size();
	totalScalarErr /= Err.size();

	std::sort(errXVector.begin(),errXVector.end());
	std::sort(errYVector.begin(),errYVector.end());
	std::sort(errZVector.begin(),errZVector.end());
	std::sort(scalarErrVector.begin(),scalarErrVector.end());
	std::sort(scalarErrXVector.begin(),scalarErrXVector.end());
	std::sort(scalarErrYVector.begin(),scalarErrYVector.end());
	std::sort(scalarErrZVector.begin(),scalarErrZVector.end());

	tex_ofs << "\\shortstack{ " << _fileName << "\\\\ 図 } &";
	/* xyz全成分について */
	ofs << "ずれベクトルのノルムの平均値," ;
	ofs << totalScalarErr << endl;
	tex_ofs << totalScalarErr << " & ";
	ofs << "ずれベクトルのノルムの中央値," ;
	ofs << scalarErrVector.at( scalarErrVector.size()/2 ) << endl;
	//tex_ofs << scalarErrVector.at( scalarErrVector.size()/2 ) << " & ";
	ofs << "ずれベクトルのノルムの最大値,";
	ofs << maxScalarErr << endl;
	tex_ofs << maxScalarErr << " & ";
	ofs << "ずれベクトルのノルムの2乗の平均値（最小化された二乗誤差）," ;
	ofs << totalSquareErr << endl;
	//tex_ofs << totalSquareErr << " & ";

	/* x,y,z各成分について */
	ofs << "x y zの各方向のずれ（符号あり）の平均値,";
	ofs << totalErrX << "," << totalErrY << "," << totalErrZ << "," << endl;
	tex_ofs << "\\shortstack{ x :  " << totalErrX << "\\\\y : " << totalErrY << "\\\\ z : " << totalErrZ << "} & " ;
	ofs << "x y zの各方向のずれ（符号あり）の中央値,";
	ofs << errXVector.at( errXVector.size()/2 ) << "," << errYVector.at( errYVector.size()/2 ) << "," << errZVector.at( errZVector.size()/2 ) << "," << endl;
	//tex_ofs << "\\shortstack{ x :  " << errXVector.at( errXVector.size()/2 ) << "\\\\y : " << errYVector.at( errYVector.size()/2 ) << "\\\\ z : " << errZVector.at( errZVector.size()/2 ) << "} & ";
	ofs << "x y zの各方向のずれのノルムの平均値,";
	ofs << totalScalarErrX << "," << totalScalarErrY << "," << totalScalarErrZ << "," << endl;
	tex_ofs << "\\shortstack{ x :  " << totalScalarErrX << "\\\\y : " << totalScalarErrY << "\\\\z : " << totalScalarErrZ << "} & ";
	ofs << "x y zの各方向のずれのノルムの中央値,";
	ofs << scalarErrXVector.at( scalarErrXVector.size()/2 ) << "," << scalarErrYVector.at( scalarErrYVector.size()/2 ) << "," << scalarErrZVector.at( scalarErrZVector.size()/2 ) << "," << endl;
	//tex_ofs << "\\shortstack{ x :  " << scalarErrXVector.at( scalarErrXVector.size()/2 ) << "\\\\y : " << scalarErrYVector.at( scalarErrYVector.size()/2 ) << "\\\\ z : " << scalarErrZVector.at( scalarErrZVector.size()/2 ) << "} & ";
	ofs << "x y zの各方向のずれのノルムの最大値,";
	ofs << maxScalarErrX << "," << maxScalarErrY << "," << maxScalarErrZ << "," << endl;
	tex_ofs << "\\shortstack{ x :  " << maxScalarErrX << "\\\\y : " << maxScalarErrY << "\\\\ z : " << maxScalarErrZ << "} " ;
	tex_ofs << "\\\\  \\hline" << endl;
	/* 各軸方向の誤差をベクトルプロットとスカラープロット形式に */
	times(j,0,3){
		static char xyz[3] = {'x','y','z'};

		//string plotFilePathS = string("plotdata/") + type1 + type2 + _fileName + xyz[j] + "_s" + ".csv";
		//ofstream tmpofsS( plotFilePathS, std::ios::out | std::ios::trunc );
		//tmpofsS << "データ形式,3" << endl;
		//tmpofsS << type1 << "から" << type2 << "の変換で，" << type2 << "内の125点における" << endl;
		//tmpofsS << xyz[j] << "方向の誤差の大きさです．" << endl;

		string plotFilePathV = string("plotdata/") + type1 + type2 + _fileName + xyz[j] + ".csv";
		ofstream tmpofsV( plotFilePathV, std::ios::out | std::ios::trunc );
		tmpofsV << "データ形式,5" << endl;
		tmpofsV << type1 << "から" << type2 << "の変換で，" << type2 << "内の125点における" << endl;
		tmpofsV << xyz[j] << "方向のずれベクトルです．" << endl;

		auto itY_all = Y_all.points.begin();
		foreach(it,Err){
			///* Scalar */
			//// ->Y
			//tmpofsS << (*itY_all)(0) << ",";
			//tmpofsS << (*itY_all)(1) << ",";
			//tmpofsS << (*itY_all)(2) << ",";
			//// P->Yの誤差(j=0(x),1(y),2(z))
			//tmpofsS << abs((*(*it))(j)) << endl;
			/* Vector */
			// ->Y
			tmpofsV << (*itY_all)(0) << ",";
			tmpofsV << (*itY_all)(1) << ",";
			tmpofsV << (*itY_all)(2) << ",";
			// P->Yの誤差(j=0(x),1(y),2(z))
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
	/* 全軸方向の誤差をベクトルプロットとスカラープロット形式に */
	{
		//string plotFilePathS = string("plotdata/") + _fileName + "_" + type1 + type2 + "xyz" + "_scalar" + ".csv";
		//ofstream tmpofsS( plotFilePathS, std::ios::out | std::ios::trunc );
		//tmpofsS << "データ形式,3" << endl;
		//tmpofsS << type1 << "から" << type2 << "の変換で，" << type2 << "内の125点における" << endl;
		//tmpofsS << "誤差の大きさです．" << endl;

		string plotFilePathV = string("plotdata/") + type1 + type2 + _fileName + ".csv";
		ofstream tmpofsV( plotFilePathV, std::ios::out | std::ios::trunc );
		tmpofsV << "データ形式,5" << endl;
		tmpofsV << type1 << "から" << type2 << "の変換で，" << type2 << "内の125点における" << endl;
		tmpofsV << "ずれのベクトルです．" << endl;

		auto itY_all = Y_all.points.begin();
		foreach(it,Err){
			///* Scalar */
			//// ->Y
			//tmpofsS << (*itY_all)(0) << ",";
			//tmpofsS << (*itY_all)(1) << ",";
			//tmpofsS << (*itY_all)(2) << ",";
			//// P->Yの誤差(j=0(x),1(y),2(z))
			//tmpofsS << (*(*it)).norm() << endl;
			/* Vector */
			// ->Y
			tmpofsV << (*itY_all)(0) << ",";
			tmpofsV << (*itY_all)(1) << ",";
			tmpofsV << (*itY_all)(2) << ",";
			// P->Yの誤差(j=0(x),1(y),2(z))
			tmpofsV << (*(*it))(0) << ",";
			tmpofsV << (*(*it))(1) << ",";
			tmpofsV << (*(*it))(2) << endl;
			itY_all++;
		}
	}
}

#include  <glut.h>
void reshape(int width, int height)
{
	static GLfloat lightPosition[4] = {0.25f, 1.0f, 0.25f, 0.0f};
	static GLfloat lightDiffuse[3] = {1.0f, 1.0f, 1.0f};
	static GLfloat lightAmbient[3] = {0.25f, 0.25f, 0.25f};
	static GLfloat lightSpecular[3] = {1.0f, 1.0f, 1.0f};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.5, 1.5, 2.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}


void display()
{
	static GLfloat diffuse[3] = {1.0f, 0.0f, 0.0f};
	static GLfloat ambient[3] = {0.25f, 0.25f, 0.25f};
	static GLfloat specular[3] = {1.0f, 1.0f, 1.0f};
	static GLfloat shininess[1] = {32.0f};

	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSolidTeapot(0.5);

	glutSwapBuffers();
}

void mario::Experiment002::writeGraphWithGL( 
	string const & _fileName, 
	mario::Experiment002::DataSet const & Y_all,
	vector< boost::shared_ptr<Eigen::Vector3d> > const& Err )
{
	int argc = 1;
	char* argv[] = {"filename"};
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Solid Teapot");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
}


void mario::Experiment002::makeDataSetFromCsv( string const & _filePath, mario::Experiment001DataList const & _dataList, DataSet & P, DataSet & Y )
{
	using namespace Eigen;
	/* 使う点を選ぶ */
	map<int,int> indexes;
	vector<Vector3d> points;
	int count = 0;
	ifstream ifs( _filePath );
	//1行分のバッファ
	string line;
	vector<string> vline;

	// csvから読み込み
	while( ifs && getline( ifs, line ) ){
		utils::cutLine( line, vline );
		int x=-1,y=-1,z=-1;

		if( vline.size() == 0 ){
			continue;
		} else if( vline.at(0) == "all" ){
			times(i,0,_dataList.l_exp001datas.size()){
				indexes.insert( make_pair(i,i) );
			}
		} else if ( vline.size()>=3 && utils::isNumber(vline.at(0)) && utils::isNumber(vline.at(1)) && utils::isNumber(vline.at(2)) ){
			x = utils::string2int(vline.at(0));
			y = utils::string2int(vline.at(1));
			z = utils::string2int(vline.at(2));
			if( x%50==0 && 0<=x && x<=200 &&
				y%50==0 && 0<=y && y<=200 &&
				z%50==0 && 0<=z && z<=200  ){
					int tmpIndex = 5*(x/50) + 25*(y/50) + (z/50);
					if( indexes.find(tmpIndex) == indexes.end() ){
						Vector3d tmp;
						tmp(0)=x; tmp(1)=y; tmp(2)=z;
						count++;
						points.push_back( tmp );
						indexes.insert( make_pair(tmpIndex,tmpIndex) );
					}
					else{
						cout <<"("<<x<<","<<y<<","<<z<<")は既に登録されています．" << endl;
					}
			}
			else{
				cout << "50の倍数かつ0～200の値で入力してください．" << endl;
			}
		}
	}
	// ソート
	{
		// ソート用ファンクタ
		struct point_compare
		{
			bool operator()(const Vector3d& lhs, const Vector3d& rhs) const {
				return 5*(lhs(0)/50)+25*(lhs(1)/50)+(lhs(2)/50) < 5*(rhs(0)/50)+25*(rhs(1)/50)+(rhs(2)/50);
			}
		};
		std::sort(points.begin(),points.end(),point_compare());
	}
	/* 点群を初期化して，ついでに重心算出 */
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

// 点群P->点群Y への剛体運動変換を求める
void mario::Experiment002::getTranslateMatrix( 
	mario::Experiment002::DataSet const & P,
	mario::Experiment002::DataSet const & Y,
	boost::shared_ptr<Eigen::Matrix3d>& _rotDst,
	boost::shared_ptr<Eigen::Vector3d>& _transDst )
{
	assert(P.points.size()==Y.points.size());
	int const Np = P.points.size();
	using namespace Eigen;
	Vector3d mu_P = P.average; // Pの重心
	Vector3d mu_Y = Y.average; // Yの重心
	Matrix3d S_PY = Matrix3d::Zero(); // PとYの共分散行列
	Matrix3d S_PY_trans = Matrix3d::Zero(); // S_PYの転置
	Vector4d q_R = Vector4d::Zero();

	/* PとYの共分散行列S_PY */
	{
		S_PY = Matrix3d::Zero(3,3);
		times(i,0,Np){
			S_PY += ( P.points.at(i) - mu_P ) * ( Y.points.at(i) - mu_Y ).transpose();
		}
		S_PY /= Np;
	}
	S_PY_trans = S_PY.transpose();

	/* q_Rをつくる */
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

	/* 回転行列R(q_R)と並進ベクトルq_Tを作る */
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

// 点群P->点群Y へのアフィン変換を求める
void mario::Experiment002::getAffineTransformation( 
	mario::Experiment002::DataSet const & P,
	mario::Experiment002::DataSet const & Y,
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

	cout << "SVD法でアフィン変換を計算中…" << endl;
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
		(*err)(0) = tmp(0) - (*itY)(0);
		(*err)(1) = tmp(1) - (*itY)(1);
		(*err)(2) = tmp(2) - (*itY)(2);
		Err.push_back(err);
		itP++;
		i++;
	}
	_errDst = Err;
}

void mario::Experiment002::getErrors( 
	mario::Experiment002::DataSet const & P,
	mario::Experiment002::DataSet const & Y,
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