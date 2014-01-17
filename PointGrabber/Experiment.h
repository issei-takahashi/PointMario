#pragma once

#include "Coordinate.h"
#include "SerializableData.h"

namespace mario
{
	class Experiment
	{
	public:
		virtual void experimentLoop() = 0;
	};
	class Experiment001 : public Experiment
	{
	public:
		class Experiment001Data : public SerializableData<Experiment001Data>
		{
			friend class boost::serialization::access;
		public:
			Coordinate<typeR> pR;
			vector<Coordinate<typeM> > v_redPointCenters;
			Coordinate<typeM> pM;
			Coordinate<typeD> pD;
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
		class Experiment001DataList : public SerializableData<Experiment001DataList>
		{
			friend class boost::serialization::access;
		public:
			list<Experiment001Data> l_exp001datas;
		private:
			template<class Archive>
			void serialize( Archive& ar, unsigned int ver )
			{
				ar & boost::serialization::make_nvp("l_exp001datas", this->l_exp001datas);
			}
		};
	public:
		void experimentLoop();
	private:
		bool inputContinueLoop();
		string inputFileNameLoop();
		Coordinate<typeR> inputRealPositionLoop();
		Coordinate<typeM> measureRedPointsLoop( vector<Coordinate<typeM> > * _pDst );
		Coordinate<typeD> showCrossAndRegisterCrossLoop();
	};
	class Experiment002 : public Experiment
	{
	public:
		void experimentLoop();
	};
};