#include "ExperimentData.h"

void mario::Experiment001Data::writeCsv( string const & _filePath )
{

}

void mario::Experiment001DataList::writeCsv( string const & _filePath )
{
	ofstream ofs( _filePath, std::ios::out | std::ios::trunc );
	ofs <<"pM.x"<<","<<"pM.y"<<","<<"pM.z"<<",";
	ofs <<"pD.x"<<","<<"pD.y"<<","<<"pD.z"<<",";
	ofs <<"pR.x"<<","<<"pR.y"<<","<<"pR.z"<<",";
	ofs <<"pR‚Ì60“_"<<","<<endl;
	foreach(it,this->l_exp001datas){
		// pM
		ofs <<it->pM.x<<","<<it->pM.y<<","<<it->pM.z<<",";
		// pD
		ofs <<it->pD.x<<","<<it->pD.y<<","<<it->pD.z<<",";
		// pR
		ofs <<it->pR.x<<","<<it->pR.y<<","<<it->pR.z<<",";
		// pR‚Ì60“_
		foreach(it2,it->v_redPointCenters){
			ofs <<it2->x<<","<<it2->y<<","<<it2->z<<",";
		}
		ofs << endl;
	}
}


void mario::Experiment001DataList::getPoints_R( vector<boost::shared_ptr<Eigen::Vector3d> > * _dst )
{
	using namespace Eigen;
	if( this->points_R.empty() ){
		foreach(it,this->l_exp001datas){
			boost::shared_ptr<Vector3d> tmp( new Vector3d() );
			(*tmp)(0) = it->pR.x;
			(*tmp)(1) = it->pR.y;
			(*tmp)(2) = it->pR.z;
			this->points_R.push_back( tmp );
		}
	}
	_dst = &this->points_R;
}

void mario::Experiment001DataList::getPoints_M( vector<boost::shared_ptr<Eigen::Vector3d> > * _dst )
{
	using namespace Eigen;
	if( this->points_M.empty() ){
		foreach(it,this->l_exp001datas){
			boost::shared_ptr<Vector3d> tmp( new Vector3d() );
			(*tmp)(0) = it->pM.x;
			(*tmp)(1) = it->pM.y;
			(*tmp)(2) = it->pM.z;
			this->points_M.push_back( tmp );
		}
	}
	_dst = &this->points_M;
}

void mario::Experiment001DataList::getPoints_D( vector<boost::shared_ptr<Eigen::Vector3d> > * _dst )
{
	using namespace Eigen;
	if( this->points_D.empty() ){
		foreach(it,this->l_exp001datas){
			boost::shared_ptr<Vector3d> tmp( new Vector3d() );
			(*tmp)(0) = it->pD.x;
			(*tmp)(1) = it->pD.y;
			(*tmp)(2) = it->pD.z;
			this->points_D.push_back( tmp );
		}
	}
	_dst = &this->points_D;
}