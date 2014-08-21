#pragma once

#include "PointBody.h"
#include "Displayed.h"

namespace mario
{
	class MidairChara : public PointBody, public Displayed<MidairChara>
	{
	public:
		static shared_ptr<MidairChara> makeShared( shared_ptr<class Application> _owner, shared_ptr<class DownOutMeasure>, string const & _folderPath );
		priority getPriority() const final override;
		void oneLoop( uint _x, uint _y ) final override;
		void setWindow( shared_ptr<Window> ) final override;
	private:
		MidairChara( shared_ptr<Application> _owner, shared_ptr<DownOutMeasure> _base, string const & _folderPath );
		map<string, shared_ptr<class Animation> > motions;
		map<string, shared_ptr<class Animation> >::iterator currentMotion;
		weak_ptr<class DownOutMeasure> const measureBase;
		frame_t frameToJump;
	};
};