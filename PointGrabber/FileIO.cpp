#include "FileIO.h"
#include "utils.h"

#define getcell(i,cells) cells.at(i)
#define getcelli(i,cells) utils::string2int(cells.at(i))
#define getcelld(i,cells) utils::string2double(cells.at(i))
#define INSERT(MAP,KEY,VAR) MAP.insert(make_pair(KEY,VAR)) 

boost::mutex             FileIO::io_mutex;
map< string, double >    FileIO::m_const;
FileIO::ConfigData       FileIO::configData;

namespace path
{
	static string const const_data = "data/const_data.csv";
	static string const config_latest = "config/latest.xml";
};

// loadPaths()で読み込んだ全てのパスのデータを読み込み
void FileIO::loadAllData()
{
		// 定数データ（csv）
	{
		FileIO::loadConst(path::const_data);
	}
	// コンフィグデータ(xml)
	{
		FileIO::loadConfigData(path::config_latest);
	}
}

// 定数の値を取得
double FileIO::getConst( string const & _name )
{
	auto it = FileIO::m_const.find( _name );

	assert( it != FileIO::m_const.end() );

	return it->second;
}

// 定数データ読み込み
void FileIO::loadConst( string const & _path )
{
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
		INSERT( FileIO::m_const, name, var );
	}
}

// パスを指定してコンフィグデータ(xml)読み込み
void FileIO::loadConfigData( string const & _path )
{
	ifstream ifs( _path );
	// ファイルがなかったら
	if( !ifs ){
		ofstream ofs( _path );
		boost::archive::xml_oarchive oa(ofs);
		// データを書き込む
		oa << boost::serialization::make_nvp( "Root", FileIO::configData );
	}
	// ファイルがあったら
	else{
		/// メモリに読み出し
		boost::archive::xml_iarchive ia(ifs);
		ia >> boost::serialization::make_nvp( "Root", FileIO::configData );
	}
}
