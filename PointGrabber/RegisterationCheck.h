#pragma once
#include "Application.h"

class RegisterationCheck : public mario::Application, public enable_shared_from_this<RegisterationCheck>
{
public:
	SHARED(RegisterationCheck);
	static shared_ptr<RegisterationCheck> makeShared( Eigen::Matrix4d const & _MtoD );
	void mainLoop() final override;
private:
	RegisterationCheck( Eigen::Matrix4d const & _MtoD );
};