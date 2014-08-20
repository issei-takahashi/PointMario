#pragma once
#include "Application.h"
#include "MidairChara.h"

class DeruChara : public mario::Application, public enable_shared_from_this<DeruChara>
{
public:
	SHARED(DeruChara);
	static shared_ptr<DeruChara> makeShared( Eigen::Matrix4d const & _MtoD );
	void mainLoop() final override;
private:
	DeruChara( Eigen::Matrix4d const & _MtoD );
private:
	list< shared_ptr<mario::MidairChara> > charas;
};