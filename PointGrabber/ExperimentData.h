#pragma once

#include "Coordinate.h"
#include "SerializableData.h"

namespace issei
{
	/* 実験データの抽象クラス */
	class ExperimentData
	{
	public:
		virtual void writeCsv( string const & _filePath ) = 0;
	};

	/* 実験1の計測1回分のデータ */
	class Experiment001Data : public ExperimentData, public issei::SerializableData<Experiment001Data>
	{
		friend class boost::serialization::access;
	public:
		mario::Coordinate<mario::typeR> pR;
		vector<mario::Coordinate<mario::typeM> > v_redPointCenters;
		mario::Coordinate<mario::typeM> pM;
		mario::Coordinate<mario::typeD> pD;
		void writeCsv( string const & _filePath );
	private:
		template<class Archive>
		void serialize( Archive& ar, unsigned int ver )
		{
			ar & boost::serialization::make_nvp("pR", this->pR);
			ar & boost::serialization::make_nvp("v_redPointCenters", this->v_redPointCenters);
			ar & boost::serialization::make_nvp("pM", this->pM);
			ar & boost::serialization::make_nvp("pD", this->pD);
		}
	};

	/* 実験1の全ての計測(60回を予定)のデータ */
	class Experiment001DataList : public ExperimentData, public issei::SerializableData<Experiment001DataList>
	{
		friend class boost::serialization::access;
	public:
		list<Experiment001Data> l_exp001datas;
		void writeCsv( string const & _filePath );
		void getPoints_R( vector<boost::shared_ptr<Eigen::Vector3d> > * _dst );
		void getPoints_M( vector<boost::shared_ptr<Eigen::Vector3d> > * _dst );
		void getPoints_D( vector<boost::shared_ptr<Eigen::Vector3d> > * _dst );
	private:
		vector<boost::shared_ptr<Eigen::Vector3d> > points_R;
		vector<boost::shared_ptr<Eigen::Vector3d> > points_M;
		vector<boost::shared_ptr<Eigen::Vector3d> > points_D;
	private:
		template<class Archive>
		void serialize( Archive& ar, unsigned int ver )
		{
			ar & boost::serialization::make_nvp("l_exp001datas", this->l_exp001datas);
		}
	};
};