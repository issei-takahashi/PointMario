// ifLine .. line �P�ʂ̃R�~�j���P�[�V�������s���ʐM������`����
// address, type, delimter ����{�I�Ȓ�`�Ƃ���

#pragma once

namespace mario{

	class ifLineError  // ifLine �z���̗�O
	{
	};

	class portDuplex : public ifLineError // ����̃|�[�g���Q�d�ɊJ�����Ƃ��ɓ��������O
	{
	private:
		int portNo;

	public:
		portDuplex() {portNo = -1; }
		portDuplex(int port) : portNo(port) {}
		int getPortNo() { return portNo; }
	};

	class ifLine
	{
	public:
		enum type   {gpib, rsline, scsi};
		enum stat   {error = 255, cannotUse = 254,
			normalEnd = 0, 
			timeOver = 1, illegalCommand, messageFull};
		enum delim  {unknown = 0, crlf = 3, cr = 1, lf = 2, eoi = 16};

	protected:
		int     hWait;  // �n�[�h�E�G�A�E�G�C�g�̂��߂� out ��
		int     myAddr;              // ��������T�|�[�g����
		type    myType;              // ����^�C�v���`����
		delim   myDelim;             // �f���~�^��`

		bool    ifaceValid;          // �������g�̎擾�ɐ��������Ƃ� ture isValid() �� void *() ���Q�Ƃ���

	public:
		enum spMode {fast, middle, slow};

		void setSpeed(spMode mod)
		{  hWait = mod; }

		virtual ~ifLine()
		{  }


		ifLine(int addr, type setIfType, delim sdelim)
		{  myAddr  = addr;
		myType  = setIfType;
		myDelim = sdelim;
		setSpeed(slow);
		ifaceValid = false;
		}

		type ifType()      { return myType; }

		virtual stat       setDelim(delim sdelim)        = 0;
		virtual stat       talk(char *message)           = 0;
		virtual stat       listen(char *message, unsigned int &maxChar) = 0;
		virtual unsigned long getStat(int mode)          {return 0; }
		virtual double     getIfVer()    {return 0; }
		virtual char      *getIfName()   {return "ifLine presents: B&C Lab.";}

		bool isValid() { return ifaceValid; }
		operator void *() {return (ifaceValid) ? this : 0; }

	};

};