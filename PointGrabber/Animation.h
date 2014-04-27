#pragma once

#include "Image.h"
#include "Displayed.h"

namespace mario{

	class OneAnimationFrame
	{
	public:
		SHARED(OneAnimationFrame);
	public:	
		OneAnimationFrame( string const& _filePath, frame_t _frame );
		Image::Ptr image;
		frame_t getFrame() const { return this->frame; }
	private:
		frame_t const frame;
	};

	class Animation : public Displayed<Animation>
	{
	public:
		SHARED(Animation);
	public:
		Animation( string const& _folderPath );
		priority getPriority() const final override;
		virtual void oneLoop( uint _x, uint _y ) override;
	protected:
		string const folderPath;
		typedef map<string, OneAnimationFrame::Ptr > frameMap_t;
		frameMap_t frames;
		frameMap_t::iterator it_frame;
		frame_t frameCount;
	};

};