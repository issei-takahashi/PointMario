#pragma once

namespace mario{
	class CollisionInterface abstract
	{
	public:
		virtual pcl::PointXYZRGBA const getSearchPoint() const = 0;
	private:

	};
};