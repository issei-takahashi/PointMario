#pragma once
#include "Application.h"

class TokyoSky1 : public mario::Application, public enable_shared_from_this<TokyoSky1>
{
public:
	SHARED(TokyoSky1);
	static shared_ptr<TokyoSky1> makeShared( Eigen::Matrix4d const & _MtoD );
	void mainLoop() final override;
private:
	TokyoSky1( Eigen::Matrix4d const & _MtoD );
};