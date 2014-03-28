#pragma once

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#define DECL_MATRIX_SERIALIZE(MAT_TYPE) \
	template<class Archive>\
	void serialize(Archive&ar,MAT_TYPE&d,unsigned int/* version */){\
	times(i,0,d.rows()){\
	times(j,0,d.cols()){\
	char is='0'+i;\
	char js='0'+j;\
	string str="("+is+","+js+")";\
	ar&make_nvp(str, d(i,j));\
				}\
			}\
		}

namespace boost { 
	namespace serialization {
		DECL_MATRIX_SERIALIZE(Eigen::Vector3d);
		DECL_MATRIX_SERIALIZE(Eigen::Vector4d);
		DECL_MATRIX_SERIALIZE(Eigen::Matrix3d);
		DECL_MATRIX_SERIALIZE(Eigen::Matrix4d);
	}
} // namespace boost::serialization