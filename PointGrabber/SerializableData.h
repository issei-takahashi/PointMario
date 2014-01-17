#pragma once

namespace mario
{
	template<class T>
	class SerializableData
	{
		friend class boost::serialization::access;
	public:
		void write( string const & _filePath, T const & instance )
		{
			std::ofstream ofs( _filePath );
			boost::archive::xml_oarchive oa(ofs);
			oa << boost::serialization::make_nvp("Root", instance );
		}
		void read( string const & _filePath, T const & instance )
		{
			std::ifstream ifs( _filePath );
			boost::archive::xml_iarchive ia(ifs);
			ia >> boost::serialization::make_nvp("Root", instance );
		}
	};
};