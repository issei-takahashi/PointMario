#pragma once
#include "Coordinate.h"
#include "DownOutMeasure.h"

namespace mario{

class Application
{
public:
	Application( Eigen::Matrix4d const & _MtoD ):MtoDMat(_MtoD),DtoMMat(_MtoD.inverse()){}
	virtual void mainLoop() = 0;
	Eigen::Matrix4d const & getMtoDMat() const { return this->MtoDMat; }
	Eigen::Matrix4d const & getDtoMMat() const { return this->DtoMMat; }
protected:
	Eigen::Matrix4d const MtoDMat;
	Eigen::Matrix4d const DtoMMat;
};

};