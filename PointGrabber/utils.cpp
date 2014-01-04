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

//ワイド文字列からマルチバイト文字列
//ロケール依存
void utils::wstring2string(const std::wstring &src, std::string &dest) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	dest = mbs;
	delete [] mbs;
}

//マルチバイト文字列からワイド文字列
//ロケール依存
void utils::string2wstring(const std::string &src, std::wstring &dest) {
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs(wcs, src.c_str(), src.length() + 1);
	dest = wcs;
	delete [] wcs;
}

// ワイド文字からマルチバイト文字
TCHAR wchar_t2TCHAR( wchar_t wchar )
{
	wstring wstr;
	wstr = wchar;
	string str;
	utils::wstring2string(wstr,str);
	return str.at(0);
}

// マルチバイト文字からワイド文字
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



//stringをdoubleに変換
double utils::string2double(const string& str)
{
	double rt;
	stringstream ss;
	ss << str;
	ss >> rt;
	return rt;
}

//stringをintに変換
int utils::string2int(const string& str)
{
	int rt;
	stringstream ss;
	ss << str;
	ss >> rt;
	return rt;
}

//doubleをstringに変換
string utils::double2string(double d)
{
	string rt;
	stringstream ss;
	ss << d;
	ss >> rt;
	return rt;
}

//intをstringに変換
string utils::int2string(int d)
{
	string rt;
	stringstream ss;
	ss << d;
	ss >> rt;
	return rt;
}

// csvの1行をセルごとにカット
void utils::cutLine( string _line, vector<string>& _dst )
{
	_dst.clear();
	if( _line == "" || _line.find("//")==0 || _line.find(",")==0 ){
		return;
	}
	// タブとスペースとクォーテーションマークをカット
	boost::algorithm::replace_all( _line, " ", "" );
	boost::algorithm::replace_all( _line, "\t", "" );
	boost::algorithm::replace_all( _line, "\"", "" );
	// 文字列_lineを","で区切って_dstに格納
	boost::algorithm::split( _dst, _line,  boost::is_any_of(",") );
}