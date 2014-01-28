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

// loadPaths()�œǂݍ��񂾑S�Ẵp�X�̃f�[�^��ǂݍ���
void mario::FileIO::loadAllData()
{
		// �萔�f�[�^�icsv�j
	{
		mario::FileIO::loadConst(path::const_data);
	}
}

// �萔�̒l���擾
double mario::FileIO::getConst( string const & _name )
{
	mario::FileIO::io_mutex.lock();

	auto it = mario::FileIO::m_const.find( _name );
	assert( it != mario::FileIO::m_const.end() );
	auto ret = it->second;

	mario::FileIO::io_mutex.unlock();
	return ret;
}

// �萔�f�[�^�ǂݍ���
void mario::FileIO::loadConst( string const & _path )
{
	mario::FileIO::io_mutex.lock();
	ifstream ifs( _path );
	//1�s���̃o�b�t�@
	string line;
	//�ŏ��̂P�s�͎̂Ă�
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

// �ϊ��f�[�^�ǂݍ���
// �萔�f�[�^�ǂݍ���
void mario::FileIO::loadTranslation( string const & _path, Eigen::Matrix3d & _dstR, Eigen::Vector3d & _dstq )
{
	mario::FileIO::io_mutex.lock();
	ifstream ifs( _path );
	//1�s���̃o�b�t�@
	string line;
	//�ŏ��̂P�s�͎̂Ă�
	getline( ifs, line );
	// ��]����
	times(i,0,3){
		getline( ifs, line );
		vector<string> cells;
		utils::cutLine( line, cells );
		times(j,0,3){
			_dstR(i,j) = utils::string2double( cells.at(j) );
		}
	}
	// 2�s�̂Ă�
	getline( ifs, line );
	getline( ifs, line );
	// ���i����
	getline( ifs, line );
	vector<string> cells;
	utils::cutLine( line, cells );
	times(i,0,3){
		_dstq(i) = utils::string2double( cells.at(i) );
	}
	mario::FileIO::io_mutex.unlock();
}

void mario::FileIO::write( string const & _path, mario::FileIO::CoordinatesData::ConstPtr & _pData )
{
	ifstream ifs( _path );
	// �t�@�C�����Ȃ�������
	if( !ifs ){
		ofstream ofs( _path );
		boost::archive::xml_oarchive oa(ofs);
		// �f�[�^����������
		oa << boost::serialization::make_nvp( "Root", *_pData );
	}
}
