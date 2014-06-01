#pragma once
#include "Application.h"

class TokyoSky2 : public mario::Application, public enable_shared_from_this<TokyoSky2>
{
public:
	SHARED(TokyoSky2);
	static shared_ptr<TokyoSky2> makeShared( Eigen::Matrix4d const & _MtoD );
	void mainLoop() final override;
private:
	TokyoSky2( Eigen::Matrix4d const & _MtoD );
};