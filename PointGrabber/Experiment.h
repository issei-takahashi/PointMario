#pragma once

#include "Coordinate.h"
#include "SerializableData.h"

namespace issei
{
	/* 実験の抽象クラス */
	class Experiment
	{
	public:
		virtual void experimentLoop() = 0;
	};

	/* 実験1 */
	class Experiment001 : public Experiment
	{
	public:
		void experimentLoop();
	private:
		bool inputContinueLoop();
		string inputFileNameLoop();
		mario::Coordinate<mario::typeR> inputRealPositionLoop();
		mario::Coordinate<mario::typeM> measureRedPointsLoop( vector<mario::Coordinate<mario::typeM> > * _pDst, mario::Coordinate<mario::typeM> const & _beforepM );
		mario::Coordinate<mario::typeD> showCrossAndRegisterCrossLoop( mario::Coordinate<mario::typeD> const & _beforepD );
	};
	
	/* 実験2(実験1データから変換行列を計算する) */
	class Experiment002 : public Experiment
	{
	public:
		void experimentLoop();
		class DataSet
		{
		public:
			DataSet( char _type )
				:type(_type),average( Eigen::Vector3d::Zero() ){}
			char type;
			vector< Eigen::Vector3d > points;
			Eigen::Vector3d average;
		};
	private:
		string inputFileNameLoop( string const & _message );
		char inputCoordinateTypeLoop( string const & _message );
		void makeDataSetFromCsv( string const & _filePath, class Experiment001DataList const & _dataList, DataSet & P, DataSet & Y );
		void writeCalculatedValues(
			string const & _filePath,
			char type1, char type2,
			issei::Experiment002::DataSet const & P,
			issei::Experiment002::DataSet const & Y,
			issei::Experiment002::DataSet const & P_all,
			issei::Experiment002::DataSet const & Y_all,
			boost::shared_ptr<Eigen::Matrix4d> const& Affine,
			vector< boost::shared_ptr<Eigen::Vector3d> > const& Err,
			ofstream & tex_ofs );
		void writeGraphWithGL( 
			string const & _fileName, 
			issei::Experiment002::DataSet const & Y_all,
			vector< boost::shared_ptr<Eigen::Vector3d> > const& Err );
		void getTranslateMatrix( 
			issei::Experiment002::DataSet const & P,
			issei::Experiment002::DataSet const & Y,
			boost::shared_ptr<Eigen::Matrix3d>& _rotDst,
			boost::shared_ptr<Eigen::Vector3d>& _transDst );
		void getAffineTransformation( 
			issei::Experiment002::DataSet const & P,
			issei::Experiment002::DataSet const & Y,
			boost::shared_ptr<Eigen::Matrix4d>& _affineDst );
		void getErrors( 
			issei::Experiment002::DataSet const & P,
			issei::Experiment002::DataSet const & Y,
			boost::shared_ptr<Eigen::Matrix3d> const& R,
			boost::shared_ptr<Eigen::Vector3d> const& q_T,
			vector< boost::shared_ptr<Eigen::Vector3d> > & Err );
		void getErrors( 
			issei::Experiment002::DataSet const & P,
			issei::Experiment002::DataSet const & Y,
			boost::shared_ptr<Eigen::Matrix4d> const& Affine,
			vector< boost::shared_ptr<Eigen::Vector3d> > & Err );
	};
	/* 実験003(実験002で計算したM->Dの変換を使って認識した位置にマーカー表示) */
	class Experiment003 : public Experiment
	{
	public:
		Experiment003( string const & _MDmatFilePath );
		void experimentLoop();
	private:
		string const MDfilePath;
	};

	/* 実験101(赤色4点抽出テストモード) */
	class Experiment101 : public Experiment
	{
	public:
		void experimentLoop();
	};

	/* 実験1001(データ修正モード) */
	class Experiment1001 : public Experiment
	{
	public:
		string inputFileNameLoop();
		void experimentLoop();
	};
	/* 実験1002(点群データキャプチャモード) */
	class Experiment1002 : public Experiment
	{
	public:
		void experimentLoop();
	};
};