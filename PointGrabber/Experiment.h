#pragma once

#include "Coordinate.h"
#include "SerializableData.h"

namespace mario
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
		Coordinate<typeR> inputRealPositionLoop();
		Coordinate<typeM> measureRedPointsLoop( vector<Coordinate<typeM> > * _pDst, Coordinate<typeM> const & _beforepM );
		Coordinate<typeD> showCrossAndRegisterCrossLoop( Coordinate<typeD> const & _beforepD );
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
		void makeDataSet( class Experiment001DataList const & _dataList, DataSet & P, DataSet & Y );
		void writeCalculatedValues(
			string const & _filePath,
			char type1, char type2,
			boost::shared_ptr<Eigen::Matrix3d> const& R,
			boost::shared_ptr<Eigen::Vector3d> const& q_T,
			vector< boost::shared_ptr<Eigen::Vector3d> > const& Err );
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
};