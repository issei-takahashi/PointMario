#pragma once


class FileIO
{
public:
	static boost::mutex io_mutex;

public:
	// 全てのデータを読み込み
	static void loadAllData();
	// 定数の値を取得
	static double getConst( string const & _name );


	// 実行中に書き換えてセーブするデータ
	class ConfigData
	{
		friend class boost::serialization::access;
	public:
		ConfigData() 
			: zoom(1){}
		double zoom;           // ブロックを100pxから何倍に拡大するか
	private:
		//シリアライズ用
		template<class Archive>
		void serialize( Archive& ar, unsigned int ver )
		{
			ar & boost::serialization::make_nvp("zoom", this->zoom);
		}
	};

	static ConfigData configData;


private:
	static map< string, double > m_const;   // 定数テーブル


private:
	// パスを指定して定数データ(csv)読み込み
	static void loadConst( string const& _path );
	// パスを指定してコンフィグデータ(xml)読み込み
	static void loadConfigData( string const & _path );

};