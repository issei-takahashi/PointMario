#pragma once
#include "Application.h"

class FallingSnow : public mario::Application, public enable_shared_from_this<FallingSnow>
{
public:
	SHARED(FallingSnow);
	static shared_ptr<FallingSnow> makeShared( Eigen::Matrix4d const & _MtoD );
	void mainLoop() final override;
private:
	FallingSnow( Eigen::Matrix4d const & _MtoD );
};