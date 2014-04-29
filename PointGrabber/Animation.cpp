#include "Animation.h"
#include "utils.h"
#include "Window.h"

using namespace mario;

/* ---------- OneAnimationFrame ---------- */

OneAnimationFrame::OneAnimationFrame( string const & _filePath, frame_t _frame, weak_ptr<Window> _pOwner )
	:frame(_frame)
{
	this->image = (shared_ptr<Image>)(new Image(_filePath));
	this->image->setWindow(_pOwner.lock());
}


/* ---------- Animation ---------- */

Animation::Animation( string const & _folderPath )
	:folderPath(_folderPath),frameCount(0)
{
	list<string> fileList;
	utils::getFileList(_folderPath,fileList);
	if( fileList.size() > 0 ){
		foreach(it,fileList){
			if( IS_IMAGE_FILE(*it) ){
				this->frames[*it] = (shared_ptr<OneAnimationFrame>)(new OneAnimationFrame(_folderPath+(*it),4,this->ownerWindow));
			}
		}
		this->it_frame = this->frames.begin();
	}
}

priority Animation::getPriority() const
{
	return 1;
}

void Animation::oneLoop( uint _x, uint _y ) 
{
	this->frameCount++;
	if( this->it_frame->second->getFrame() < this->frameCount ){
		auto tmpit = this->it_frame++;
		if( tmpit == this->frames.end() ){
			tmpit = this->frames.begin();
		}
		this->it_frame = tmpit;
		this->frameCount = 0;
	}
	this->it_frame->second->image->oneLoop(_x,_y);
}

