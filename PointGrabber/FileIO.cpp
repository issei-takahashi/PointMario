#include "FileIO.h"
#include "utils.h"

#define getcell(i,cells) cells.at(i)
#define getcelli(i,cells) utils::string2int(cells.at(i))
#define getcelld(i,cells) utils::string2double(cells.at(i))
#define INSERT(MAP,KEY,VAR) MAP.insert(make_pair(KEY,VAR)) 

boost::mutex             mario::FileIO::io_mutex;
map< string, double >    mario::FileIO::m_const;
mario::FileIO::CoordinatesData       mario::FileIO::coordinatesData;

namespace path
{
	static string const const_data = "data/const_data.csv";
};

mario::FileIO::CoordinatesData::CoordinatesData()
{}

// loadPaths()で読み込んだ全てのパスのデータを読み込み
void mario::FileIO::loadAllData()
{
		// 定数データ（csv）
	{
		mario::FileIO::loadConst(path::const_data);
	}
}

// 定数の値を取得
double mario::FileIO::getConst( string const & _name )
{
	mario::FileIO::io_mutex.lock();

	auto it = mario::FileIO::m_const.find( _name );
	assert( it != mario::FileIO::m_const.end() );
	auto ret = it->second;

	mario::FileIO::io_mutex.unlock();
	return ret;
}

// 定数データ読み込み
void mario::FileIO::loadConst( string const & _path )
{
	mario::FileIO::io_mutex.lock();
	ifstream ifs( _path );
	//1行分のバッファ
	string line;
	//最初の１行は捨てる
	getline( ifs, line );

	int lineCount = -1;
	while( ifs && getline( ifs, line ) ){
		lineCount++;
		vector<string> cells;
		utils::cutLine( line, cells );
		int i=0;
		if( cells.size() <= 1 ){ 
			continue;
		}
		string name = getcell(i++,cells);
		double var = getcelld(i++,cells);
		INSERT( mario::FileIO::m_const, name, var );
	}
	mario::FileIO::io_mutex.unlock();
}

// 変換データ読み込み
// 定数データ読み込み
void mario::FileIO::loadMatrix( string const & _path, Eigen::Matrix4d & _A )
{
	mario::FileIO::io_mutex.lock();
	ifstream ifs( _path );
	//1行分のバッファ
	string line;
	// 回転成分
	times(i,0,4){
		getline( ifs, line );
		vector<string> cells;
		utils::cutLine( line, cells );
		times(j,0,4){
			_A(i,j) = utils::string2double( cells.at(j) );
		}
	}
	mario::FileIO::io_mutex.unlock();
}

void mario::FileIO::writeMatrix( string const & _path, Eigen::Matrix4d const & _A )
{
	mario::FileIO::io_mutex.lock();
	ofstream ofs( _path );
	//1行分のバッファ
	string line;
	times(i,0,_A.rows()){
		times(j,0,_A.cols()){
			ofs << _A(i,j);
			if( j < _A.cols()-1 ){
				ofs << ",";
			}else{
				ofs << endl;
			}
		}
	}
	mario::FileIO::io_mutex.unlock();
}

void mario::FileIO::write( string const & _path, mario::FileIO::CoordinatesData::ConstPtr & _pData )
{
	ifstream ifs( _path );
	// ファイルがなかったら
	if( !ifs ){
		ofstream ofs( _path );
		boost::archive::xml_oarchive oa(ofs);
		// データを書き込む
		oa << boost::serialization::make_nvp( "Root", *_pData );
	}
}
