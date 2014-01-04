#include "utils.h"

bool utils::fileExist( string const & _fileName )
{
	FILE  *fp;

	if ((fp = fopen(_fileName.c_str(), "r")) == NULL)
		return false;
	fclose(fp);
	return true;
}

bool utils::randomEvent( double _prop )
{
	if( ((double)rand())/RAND_MAX <= _prop ){
		return true;
	}
	else{
		return false;
	}
}

//���C�h�����񂩂�}���`�o�C�g������
//���P�[���ˑ�
void utils::wstring2string(const std::wstring &src, std::string &dest) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	dest = mbs;
	delete [] mbs;
}

//�}���`�o�C�g�����񂩂烏�C�h������
//���P�[���ˑ�
void utils::string2wstring(const std::string &src, std::wstring &dest) {
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs(wcs, src.c_str(), src.length() + 1);
	dest = wcs;
	delete [] wcs;
}

// ���C�h��������}���`�o�C�g����
TCHAR wchar_t2TCHAR( wchar_t wchar )
{
	wstring wstr;
	wstr = wchar;
	string str;
	utils::wstring2string(wstr,str);
	return str.at(0);
}

// �}���`�o�C�g�������烏�C�h����
wchar_t TCHAR2w_char( TCHAR tchar )
{
	string str;
	str = tchar;
	wstring wstr;
	utils::string2wstring(str,wstr);
	return wstr.at(0);
}

double utils::random( double var1, double var2 )
{
	return var1 + ((double)rand()/RAND_MAX) * ( var2-var1 );
}



//string��double�ɕϊ�
double utils::string2double(const string& str)
{
	double rt;
	stringstream ss;
	ss << str;
	ss >> rt;
	return rt;
}

//string��int�ɕϊ�
int utils::string2int(const string& str)
{
	int rt;
	stringstream ss;
	ss << str;
	ss >> rt;
	return rt;
}

//double��string�ɕϊ�
string utils::double2string(double d)
{
	string rt;
	stringstream ss;
	ss << d;
	ss >> rt;
	return rt;
}

//int��string�ɕϊ�
string utils::int2string(int d)
{
	string rt;
	stringstream ss;
	ss << d;
	ss >> rt;
	return rt;
}

// csv��1�s���Z�����ƂɃJ�b�g
void utils::cutLine( string _line, vector<string>& _dst )
{
	_dst.clear();
	if( _line == "" || _line.find("//")==0 || _line.find(",")==0 ){
		return;
	}
	// �^�u�ƃX�y�[�X�ƃN�H�[�e�[�V�����}�[�N���J�b�g
	boost::algorithm::replace_all( _line, " ", "" );
	boost::algorithm::replace_all( _line, "\t", "" );
	boost::algorithm::replace_all( _line, "\"", "" );
	// ������_line��","�ŋ�؂���_dst�Ɋi�[
	boost::algorithm::split( _dst, _line,  boost::is_any_of(",") );
}