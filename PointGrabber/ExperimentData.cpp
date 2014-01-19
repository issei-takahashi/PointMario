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