#pragma once

#include "Coordinate.h"
#include "ColorRGB.h"

namespace mario{
	typedef boost::shared_ptr< vector<int> > indices_t;
	typedef boost::shared_ptr< std::vector<float> > distances_t;

	void filterA( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	bool detectMarkers( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst, list< pcl::PointCloud<pcl::PointXYZRGBA>::Ptr >& l_dst, Coordinate<typeM> & jigCenter );
	void downSamplingFilter( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void outlierFilter( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	Coordinate<typeM> redDetection(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void redExtraction(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void greenExtraction(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void colorExtractionHSV( ColorRGB const & color, uint hTh, uint sTh, uint vTh, const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst );
	void removePlane( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst, double threshould );
	void cvt2Mat( const boost::shared_ptr<openni_wrapper::Image>& input, boost::shared_ptr<openni_wrapper::Image>& output );
	void clusterize( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst, list< pcl::PointCloud<pcl::PointXYZRGBA>::Ptr >& l_dst, int maxNum );

	bool simpleNeighborsSearchAndGetHighestPoint( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _inputCloud,
		pcl::PointXYZRGBA const & _searchPoint, float _distance, indices_t & _ind, mario::Coordinate<mario::typeD> & _highDst, int& _highIndex );

	bool searchNeighbors_simple( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _inputCloud,
		pcl::PointXYZRGBA const & _searchPoint, float _distance, indices_t & _ind );

	bool searchNeighbors_voxel( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _inputCloud,
		pcl::PointXYZRGBA const & _searchPoint, float _resolution, indices_t & _ind );

	bool searchNeighbors_Knearest( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _inputCloud,
		pcl::PointXYZRGBA const & _searchPoint, float _resolution, int _K, indices_t & _ind, distances_t & _dist );

	bool searchNeighbors_radius( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _inputCloud,
		pcl::PointXYZRGBA const & _searchPoint, float _resolution, float _radius, indices_t & _ind, distances_t & _dist );

	void newPointSearch( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _cloudA,
		const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _cloudB, float _resolution, indices_t & _ind);

	Coordinate<typeM> getAverage( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud );
}
