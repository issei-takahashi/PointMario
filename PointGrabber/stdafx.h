// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#include <string>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <queue>
#include <stack>
//#include <memory>
#include <fstream>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <functional>
#include <sstream>


//#include <string.h>
//#include <Windows.h>
#include <math.h>
//#include <process.h>
#include  <time.h>

/* ---------- PCL ---------- */
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/io/openni_grabber.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/passthrough.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/filters/approximate_voxel_grid.h>
// visualizer
#include <pcl/common/common_headers.h>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>
// openni_viwer
#include <pcl/common/time.h> //fps calculations
#include <pcl/io/openni_grabber.h>
#include <pcl/visualization/point_cloud_handlers.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/image_viewer.h>
#include <pcl/console/print.h>
#include <pcl/console/parse.h>
#include <pcl/console/time.h>

#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/console/parse.h>
#include <pcl/visualization/cloud_viewer.h>


#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>

 #include "pcl/segmentation/sac_segmentation.h"

// Sample Consensus methods
#include "pcl/sample_consensus/method_types.h"
#include "pcl/sample_consensus/sac.h"
#include "pcl/sample_consensus/lmeds.h"
#include "pcl/sample_consensus/mlesac.h"
#include "pcl/sample_consensus/msac.h"
#include "pcl/sample_consensus/ransac.h"
#include "pcl/sample_consensus/rmsac.h"
#include "pcl/sample_consensus/rransac.h"

// Sample Consensus models
#include "pcl/sample_consensus/model_types.h"
#include "pcl/sample_consensus/sac_model.h"
#include "pcl/sample_consensus/sac_model_circle.h"
#include "pcl/sample_consensus/sac_model_cylinder.h"
#include "pcl/sample_consensus/sac_model_line.h"
#include "pcl/sample_consensus/sac_model_normal_plane.h"
#include "pcl/sample_consensus/sac_model_parallel_plane.h"
#include "pcl/sample_consensus/sac_model_normal_parallel_plane.h"
#include "pcl/sample_consensus/sac_model_parallel_line.h"
#include "pcl/sample_consensus/sac_model_perpendicular_plane.h"
#include "pcl/sample_consensus/sac_model_plane.h"
#include "pcl/sample_consensus/sac_model_sphere.h"


/* ---------- boost ---------- */
#include <boost/thread/thread.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp> 
#include <boost/serialization/list.hpp> 
#include <boost/serialization/vector.hpp> 
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>    // テキスト形式アーカイブに書き込み
#include <boost/archive/text_iarchive.hpp>    // テキスト形式アーカイブから読み込み
#include <boost/archive/binary_oarchive.hpp>  // バイナリ形式アーカイブに書き込み
#include <boost/archive/binary_iarchive.hpp>  // バイナリ形式アーカイブから読み込み

#include <boost/algorithm/string.hpp>

#include <boost/lambda/lambda.hpp>

#include <boost/make_shared.hpp>
#include <boost/shared_array.hpp>

/* OpenCV1 */
#include <opencv/cv.h>  
#include <opencv/highgui.h>

/* ---------- OpenCV2.2 ---------- */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/* SDL */
#include <SDL.h>
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#include <SDL_image.h>
#pragma comment(lib, "SDL_image.lib")
#include <SDL_ttf.h>
#pragma comment(lib, "SDL_ttf.lib")

/* Eigen */
#include <Eigen/Core>
#include <Eigen/Dense>
///#include <Eigen/Sparse>