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

// loadPaths()�œǂݍ��񂾑S�Ẵp�X�̃f�[�^��ǂݍ���
void FileIO::loadAllData()
{
		// �萔�f�[�^�icsv�j
	{
		FileIO::loadConst(path::const_data);
	}
	// �R���t�B�O�f�[�^(xml)
	{
		FileIO::loadConfigData(path::config_latest);
	}
}

// �萔�̒l���擾
double FileIO::getConst( string const & _name )
{
	auto it = FileIO::m_const.find( _name );

	assert( it != FileIO::m_const.end() );

	return it->second;
}

// �萔�f�[�^�ǂݍ���
void FileIO::loadConst( string const & _path )
{
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
		INSERT( FileIO::m_const, name, var );
	}
}

// �p�X���w�肵�ăR���t�B�O�f�[�^(xml)�ǂݍ���
void FileIO::loadConfigData( string const & _path )
{
	ifstream ifs( _path );
	// �t�@�C�����Ȃ�������
	if( !ifs ){
		ofstream ofs( _path );
		boost::archive::xml_oarchive oa(ofs);
		// �f�[�^����������
		oa << boost::serialization::make_nvp( "Root", FileIO::configData );
	}
	// �t�@�C������������
	else{
		/// �������ɓǂݏo��
		boost::archive::xml_iarchive ia(ifs);
		ia >> boost::serialization::make_nvp( "Root", FileIO::configData );
	}
}
