#pragma once


namespace utils
{

	bool fileExist( string const & _fileName );

	bool randomEvent( double _prop );

	// ���C�h�����񂩂�}���`�o�C�g������
	///���P�[���ˑ�
	void wstring2string(const std::wstring &src, std::string &dest);

	// �}���`�o�C�g�����񂩂烏�C�h������
	///���P�[���ˑ�
	void string2wstring(const std::string &src, std::wstring &dest);

	// ���C�h��������}���`�o�C�g����
	TCHAR wchar_t2TCHAR( wchar_t wchar );

	// �}���`�o�C�g�������烏�C�h����
	wchar_t TCHAR2w_char( TCHAR tchar );

	// var1�`var2�̊Ԃ̗�������
	double random( double var1, double var2 );

	
	//string��double�ɕϊ�
	double string2double(const string& str);

	//string��int�ɕϊ�
	int string2int(const string& str);

	//double��string�ɕϊ�
	string double2string(double d);

	//int��string�ɕϊ�
	string int2string(int d);

	// csv��1�s���Z�����ƂɃJ�b�g
	void cutLine( string _line, vector<string>& _dst );
};