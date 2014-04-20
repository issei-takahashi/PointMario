#pragma once

#include "Animation.h"
#include "CollisionInterface.h"

namespace mario{
	class SolidAnimation : public Animation, public CollisionInterface
	{
	public:
		SHARED(SolidAnimation);
	public:
		static SolidAnimation::Ptr makeShared( string const& _folderPath );
	private:
		SolidAnimation( string const& _folderPath );
	};
};