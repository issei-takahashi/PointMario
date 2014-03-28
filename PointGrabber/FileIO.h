#pragma once

#include "Coordinate.h"

namespace mario{

	class FileIO
	{
	public:
		static boost::mutex io_mutex;

	public:
		static void loadAllData();
		static double getConst( string const & _name );
		static void loadMatrix( string const & _path, Eigen::Matrix4d & _A );
		static void writeMatrix( string const & _path, Eigen::Matrix4d const & _A );
	public: 
		class CoordinatesData{
			friend class boost::serialization::access;
		public:
			CoordinatesData();
		private:
			Coordinate<typeR> pointR;
			Coordinate<typeD> pointD;
			vector<typeM> v_pointM;
		private:
			template<class Archive>
			void serialize( Archive& ar, unsigned int ver )
			{
				ar & boost::serialization::make_nvp("pointR", this->pointR);
				ar & boost::serialization::make_nvp("pointD", this->pointD);
				ar & boost::serialization::make_nvp("v_pointM", this->v_pointM);
			}
		public:
			typedef boost::shared_ptr<CoordinatesData> Ptr;
			typedef boost::shared_ptr<const CoordinatesData> ConstPtr;
		};

	private:
		static map< string, double > m_const;   // íËêîÉeÅ[ÉuÉã
		static CoordinatesData coordinatesData;

	private:
		static void loadConst( string const& _path );
		static void write( string const & _path, mario::FileIO::CoordinatesData::ConstPtr & _pData );
	};

};