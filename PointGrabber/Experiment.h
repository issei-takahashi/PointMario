#pragma once

#include "Coordinate.h"
#include "SerializableData.h"

namespace issei
{
	/* �����̒��ۃN���X */
	class Experiment
	{
	public:
		virtual void experimentLoop() = 0;
	};

	/* ����1 */
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
	
	/* ����2(����1�f�[�^����ϊ��s����v�Z����) */
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
	/* ����003(����002�Ōv�Z����M->D�̕ϊ����g���ĔF�������ʒu�Ƀ}�[�J�[�\��) */
	class Experiment003 : public Experiment
	{
	public:
		void experimentLoop();
	};

	/* ����101(�ԐF4�_���o�e�X�g���[�h) */
	class Experiment101 : public Experiment
	{
	public:
		void experimentLoop();
	};

	/* ����1001(�f�[�^�C�����[�h) */
	class Experiment1001 : public Experiment
	{
	public:
		string inputFileNameLoop();
		void experimentLoop();
	};
	/* ����1002(�_�Q�f�[�^�L���v�`�����[�h) */
	class Experiment1002 : public Experiment
	{
	public:
		void experimentLoop();
	};
};