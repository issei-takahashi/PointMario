#pragma once

#include "Image.h"
#include "Displayed.h"

namespace mario{

	class OneAnimationFrame
	{
	public:
		SHARED(OneAnimationFrame);
	public:	
		OneAnimationFrame( string const& _filePath, frame_t _frame, weak_ptr<class Window> _pOwner );
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
		static shared_ptr<Animation> makeShared( string const& _folderPath );
		priority getPriority() const final override;
		virtual void oneLoop( uint _x, uint _y ) override;
		void displayStart() final override;
		void displayStop() final override;
		void setWindow( shared_ptr<Window> ) final override;
	protected:
		Animation( string const& _folderPath );
		string const folderPath;
		typedef map<string, OneAnimationFrame::Ptr > frameMap_t;
		frameMap_t frames;
		frameMap_t::iterator it_frame;
		frame_t frameCount;
	};

};