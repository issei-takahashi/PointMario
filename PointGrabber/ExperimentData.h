#pragma once

#include "Coordinate.h"
#include "SerializableData.h"

namespace mario
{
	/* 実験データの抽象クラス */
	class ExperimentData
	{
	public:
		virtual void writeCsv( string const & _filePath ) = 0;
	};

	/* 実験1の計測1回分のデータ */
	class Experiment001Data : public ExperimentData, public SerializableData<Experiment001Data>
	{
		friend class boost::serialization::access;
	public:
		Coordinate<typeR> pR;
		vector<Coordinate<typeM> > v_redPointCenters;
		Coordinate<typeM> pM;
		Coordinate<typeD> pD;
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
	class Experiment001DataList : public ExperimentData, public SerializableData<Experiment001DataList>
	{
		friend class boost::serialization::access;
	public:
		list<Experiment001Data> l_exp001datas;
		void writeCsv( string const & _filePath );
	private:
		template<class Archive>
		void serialize( Archive& ar, unsigned int ver )
		{
			ar & boost::serialization::make_nvp("l_exp001datas", this->l_exp001datas);
		}
	};
};