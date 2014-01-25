#pragma once

#include "Coordinate.h"
#include "SerializableData.h"

namespace mario
{
	/* �����f�[�^�̒��ۃN���X */
	class ExperimentData
	{
	public:
		virtual void writeCsv( string const & _filePath ) = 0;
	};

	/* ����1�̌v��1�񕪂̃f�[�^ */
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

	/* ����1�̑S�Ă̌v��(60���\��)�̃f�[�^ */
	class Experiment001DataList : public ExperimentData, public SerializableData<Experiment001DataList>
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