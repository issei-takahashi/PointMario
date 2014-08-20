#include "MidairChara.h"
#include "Animation.h"
#include "Application.h"
#include "utils.h"

using namespace mario;

MidairChara::MidairChara( shared_ptr<class Application> _owner, string const & _folderPath )
	:PointBody(_owner)
{
	list<string> fileList;
	utils::getFileList(_folderPath,fileList); // �t�H���_�̃��X�g���擾
	if( fileList.size() > 0 ){
		foreach(it,fileList){ // �e�t�H���_�ɑ΂���
			if( !IS_IMAGE_FILE(*it) ){
				this->motions[*it] = Animation::makeShared(_folderPath+*it+"/"); // �A�j���[�V����������ēo�^
			}
		}
		this->currentMotion = this->motions.begin();
	}
}

shared_ptr<MidairChara> MidairChara::makeShared( shared_ptr<class Application> _owner, string const & _folderPath )
{
	return (shared_ptr<MidairChara>)(new MidairChara( _owner, _folderPath) );
}

priority MidairChara::getPriority() const
{
	return 1;
}

void MidairChara::oneLoop( uint _x, uint _y ) 
{
	if( this->currentMotion != this->motions.end() ){
		this->currentMotion->second->oneLoop(_x,_y);
	}
}

void MidairChara::setWindow( shared_ptr<Window> _owner )
{
	foreach(it,this->motions){
		it->second->setWindow(_owner);
	}
}