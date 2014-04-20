#pragma once

#include "Singleton.h"
#include "Coordinate.h"

namespace mario
{
	class Calibration : public Singleton<Calibration>
	{
		friend class Singleton<Calibration>;
	public:
		class DataSet
		{
		public:
			DataSet( char _type )
				:type(_type),average( Eigen::Vector3d::Zero() ){}
			char type;
			vector< Eigen::Vector3d > points;
			Eigen::Vector3d average;
		};
	public:
		void executeCalibration( list<Coordinate<typeD> > const & _calibPoints, string const & _filePath );
	private:
		Calibration(){}

		void writeCalculatedValues(
			string const & _filePath,
			char type1, char type2,
			DataSet const & P,
			DataSet const & Y,
			DataSet const & P_all,
			DataSet const & Y_all,
			boost::shared_ptr<Eigen::Matrix4d> const& Affine,
			vector< boost::shared_ptr<Eigen::Vector3d> > const& Err,
			ofstream & tex_ofs );
		void getTranslateMatrix( 
			DataSet const & P,
			DataSet const & Y,
			boost::shared_ptr<Eigen::Matrix3d>& _rotDst,
			boost::shared_ptr<Eigen::Vector3d>& _transDst );
		void getAffineTransformation( 
			DataSet const & P,
			DataSet const & Y,
			boost::shared_ptr<Eigen::Matrix4d>& _affineDst );
		void getErrors( 
			DataSet const & P,
			DataSet const & Y,
			boost::shared_ptr<Eigen::Matrix3d> const& R,
			boost::shared_ptr<Eigen::Vector3d> const& q_T,
			vector< boost::shared_ptr<Eigen::Vector3d> > & Err );
		void getErrors( 
			DataSet const & P,
			DataSet const & Y,
			boost::shared_ptr<Eigen::Matrix4d> const& Affine,
			vector< boost::shared_ptr<Eigen::Vector3d> > & Err );

	};
};