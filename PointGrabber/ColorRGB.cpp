#include "ColorRGB.h"

using namespace mario;

uint ColorRGB::getH() const
{
	uint mx = max(this->R, max(this->G, this->B));
	uint mn = min(this->R, min(this->G, this->B));
	if(mx == mn){
		return 0;
	}else if(mx == this->R){
		return (60 * (this->G - this->B) / (mx - mn) + 360) % 360;
	}else if(mx == this->G){
		return (60 * (this->B - this->R) / (mx - mn)) + 120;
	}else if(mx == this->B){
		return (60 * (this->R - this->G) / (mx - mn)) + 240;   
	}else{
		return 0;
	}
}

uint ColorRGB::getS() const
{
	uint mx = max(this->R, max(this->G, this->B));
	uint mn = min(this->R, min(this->G, this->B));
	if(mx == 0){
		return 0;
	}else{
		return (255 * ((mx - mn) / mx));
	}
}

uint ColorRGB::getV() const
{
	uint mx = max(this->R, max(this->G, this->B));
	uint mn = min(this->R, min(this->G, this->B));
	return mx;
}
