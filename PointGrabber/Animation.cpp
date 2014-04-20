#include "Animation.h"
#include "utils.h"

using namespace mario;

/* ---------- OneAnimationFrame ---------- */

OneAnimationFrame::OneAnimationFrame( string const & _filePath, frame_t _frame )
	:frame(_frame)
{
	this->image = Image::makeShared(_filePath);
}

OneAnimationFrame::Ptr OneAnimationFrame::makeShared( string const& _filePath, frame_t _frame )
{
	return (OneAnimationFrame::Ptr)( new OneAnimationFrame(_filePath,_frame) );
}


/* ---------- Animation ---------- */

shared_ptr<Animation> Animation::makeShared( string const & _folderPath )
{
	return shared_ptr<Animation>( new Animation(_folderPath) );
}

Animation::Animation( string const & _folderPath )
	:folderPath(_folderPath),frameCount(0)
{
	list<string> fileList;
	utils::getFileList(_folderPath,fileList);
	if( fileList.size() > 0 ){
		foreach(it,fileList){
			if( IS_IMAGE_FILE(*it) ){
				this->frames[*it] = OneAnimationFrame::makeShared(*it,4);
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

