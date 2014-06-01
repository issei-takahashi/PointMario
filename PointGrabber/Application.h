#pragma once
#include "Coordinate.h"
#include "DownOutMeasure.h"

namespace mario{

class Application
{
public:
	Application( Eigen::Matrix4d const & _MtoD )
		:MtoDMat(_MtoD){}
	virtual void mainLoop() = 0;
	Eigen::Matrix4d const & getMtoDMat() const { return this->MtoDMat; }
protected:
	Eigen::Matrix4d const MtoDMat;
};

};