#pragma once

#include "Image.h"
#include "Displayed.h"

namespace mario{

	class OneAnimationFrame
	{
	public:
		SHARED(OneAnimationFrame);
	public:		
		static OneAnimationFrame::Ptr makeShared( string const& _filePath, frame_t _frame );
		Image::Ptr image;
		frame_t getFrame() const { return this->frame; }
	private:
		OneAnimationFrame( string const& _filePath, frame_t _frame );
		frame_t const frame;
	};

	class Animation : public Displayed<Animation>
	{
	public:
		SHARED(Animation);
	public:
		static shared_ptr<Animation> makeShared( string const & _fileName );
		priority getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
	protected:
		Animation( string const& _folderPath );
		string const folderPath;
		typedef map<string, OneAnimationFrame::Ptr > frameMap_t;
		frameMap_t frames;
		frameMap_t::iterator it_frame;
		frame_t frameCount;
	};

};