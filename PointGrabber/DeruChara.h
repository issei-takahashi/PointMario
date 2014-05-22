#pragma once
#include "Coordinate.h"
#include "DownOutMeasure.h"

class DeruChara
{
public:
	DeruChara( Eigen::Matrix4d const & _MtoD );
	void mainLoop();
private:
	Eigen::Matrix4d const MtoDMat;
	//mario::DownOutMeasure downOutMeasure;
};