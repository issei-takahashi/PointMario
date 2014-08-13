#pragma once

namespace mario{
	class SolidBody abstract
	{
	public:
		SolidBody( shared_ptr<class Application> _owner )
		:ownerApp(_owner){}
		virtual pcl::PointXYZRGBA const getSearchPoint() = 0;
	protected:
		weak_ptr<class Application> const ownerApp;
	};
};