#pragma once


class FileIO
{
public:
	static boost::mutex io_mutex;

public:
	// �S�Ẵf�[�^��ǂݍ���
	static void loadAllData();
	// �萔�̒l���擾
	static double getConst( string const & _name );


	// ���s���ɏ��������ăZ�[�u����f�[�^
	class ConfigData
	{
		friend class boost::serialization::access;
	public:
		ConfigData() 
			: zoom(1){}
		double zoom;           // �u���b�N��100px���牽�{�Ɋg�傷�邩
	private:
		//�V���A���C�Y�p
		template<class Archive>
		void serialize( Archive& ar, unsigned int ver )
		{
			ar & boost::serialization::make_nvp("zoom", this->zoom);
		}
	};

	static ConfigData configData;


private:
	static map< string, double > m_const;   // �萔�e�[�u��


private:
	// �p�X���w�肵�Ē萔�f�[�^(csv)�ǂݍ���
	static void loadConst( string const& _path );
	// �p�X���w�肵�ăR���t�B�O�f�[�^(xml)�ǂݍ���
	static void loadConfigData( string const & _path );

};