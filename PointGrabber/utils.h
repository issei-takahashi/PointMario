#pragma once


namespace utils
{

	bool fileExist( string const & _fileName );

	bool randomEvent( double _prop );

	// ワイド文字列からマルチバイト文字列
	///ロケール依存
	void wstring2string(const std::wstring &src, std::string &dest);

	// マルチバイト文字列からワイド文字列
	///ロケール依存
	void string2wstring(const std::string &src, std::wstring &dest);

	// ワイド文字からマルチバイト文字
	TCHAR wchar_t2TCHAR( wchar_t wchar );

	// マルチバイト文字からワイド文字
	wchar_t TCHAR2w_char( TCHAR tchar );

	// var1～var2の間の乱数発生
	double random( double var1, double var2 );

	
	//stringをdoubleに変換
	double string2double(const string& str);

	//stringをintに変換
	int string2int(const string& str);

	//doubleをstringに変換
	string double2string(double d);

	//intをstringに変換
	string int2string(int d);

	// csvの1行をセルごとにカット
	void cutLine( string _line, vector<string>& _dst );
};