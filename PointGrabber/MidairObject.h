#pragma once

namespace mario
{
	template<class BODY,class DISPLAYED>
	class MidairObject
	{
	public:
		MidairObject(){}
		MidairObject(shared_ptr<BODY> bd, shared_ptr<DISPLAYED> dp)
			:body(bd),displayed(dp){}
		shared_ptr<BODY> getBody() const {return this->body;}
		shared_ptr<DISPLAYED> getDisplayed() const {return displayed;}
	private:
		shared_ptr<BODY> body;
		shared_ptr<DISPLAYED> displayed; 
	};
};