#include "Eventer.h"

#include <SDL.h>

bool mario::Eventer::quitEvent()
{
	SDL_Event Qevnts;
	if( SDL_PollEvent( &Qevnts ) ){
		switch( Qevnts.type ){
		case SDL_QUIT:
			{
				return true;
				break;
			}
		case SDL_KEYDOWN:
			{
				if( Qevnts.key.keysym.sym==SDLK_ESCAPE ){
					return true;
				}
				break;
			}
		default: 
			{
				break;
			}
		}
	}
	return false;
}

int getSDLKey( mario::KeyType _key )
{
	switch(_key){
	case mario::KeyType::KEY_LEFT:
		return SDLK_LEFT;
	case mario::KeyType::KEY_RIGHT:
		return SDLK_RIGHT;
	case mario::KeyType::KEY_UP:
		return SDLK_UP;
	case mario::KeyType::KEY_DOWN:
		return SDLK_DOWN;
	case mario::KeyType::KEY_RSHIFT:
		return SDLK_RSHIFT;
	case mario::KeyType::KEY_LSHIFT:
		return SDLK_LSHIFT;
	case mario::KeyType::KEY_RCTRL:
		return SDLK_RCTRL;
	case mario::KeyType::KEY_LCTRL:
		return SDLK_LCTRL;
	case mario::KeyType::KEY_RALT:
		return SDLK_RALT;
	case mario::KeyType::KEY_LALT:
		return SDLK_LALT;
	case mario::KeyType::KEY_RETURN:
		return SDLK_RETURN;
	case mario::KeyType::KEY_A:
		return SDLK_a;
	case mario::KeyType::KEY_B:
		return SDLK_b;
	case mario::KeyType::KEY_C:
		return SDLK_c;
	case mario::KeyType::KEY_D:
		return SDLK_d;
	case mario::KeyType::KEY_E:
		return SDLK_e;
	case mario::KeyType::KEY_F:
		return SDLK_f;
	case mario::KeyType::KEY_G:
		return SDLK_g;
	case mario::KeyType::KEY_H:
		return SDLK_h;
	case mario::KeyType::KEY_I:
		return SDLK_i;
	case mario::KeyType::KEY_J:
		return SDLK_j;
	case mario::KeyType::KEY_K:
		return SDLK_k;
	case mario::KeyType::KEY_L:
		return SDLK_l;
	case mario::KeyType::KEY_M:
		return SDLK_m;
	case mario::KeyType::KEY_N:
		return SDLK_n;
	case mario::KeyType::KEY_O:
		return SDLK_o;
	case mario::KeyType::KEY_P:
		return SDLK_p;
	case mario::KeyType::KEY_Q:
		return SDLK_q;
	case mario::KeyType::KEY_R:
		return SDLK_r;
	case mario::KeyType::KEY_S:
		return SDLK_s;
	case mario::KeyType::KEY_T:
		return SDLK_t;
	case mario::KeyType::KEY_U:
		return SDLK_u;
	case mario::KeyType::KEY_V:
		return SDLK_v;
	case mario::KeyType::KEY_W:
		return SDLK_w;
	case mario::KeyType::KEY_X:
		return SDLK_x;
	case mario::KeyType::KEY_Y:
		return SDLK_y;
	case mario::KeyType::KEY_Z:
		return SDLK_z;
	case mario::KeyType::KEY_0:
		return SDLK_0;
	case mario::KeyType::KEY_1:
		return SDLK_1;
	case mario::KeyType::KEY_2:
		return SDLK_2;
	case mario::KeyType::KEY_3:
		return SDLK_3;
	case mario::KeyType::KEY_4:
		return SDLK_4;
	case mario::KeyType::KEY_5:
		return SDLK_5;
	case mario::KeyType::KEY_6:
		return SDLK_6;
	case mario::KeyType::KEY_7:
		return SDLK_7;
	case mario::KeyType::KEY_8:
		return SDLK_8;
	case mario::KeyType::KEY_9:
		return SDLK_9;
	default:
		return INT_MAX;
	}
}

bool mario::Eventer::keyEvent( mario::KeyType _key )
{
	Uint8 *getKeys = SDL_GetKeyState(NULL);
	int sdlk = ::getSDLKey(_key);
	int size = sizeof(getKeys)/sizeof(Uint8);
	if(sdlk<size){
		return (getKeys[sdlk]==SDL_PRESSED);
	}else{
		return false;
	}
}
