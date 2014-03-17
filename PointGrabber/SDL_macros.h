#pragma once

#define CLEAR_WINDOW(_SURFACE_) SDL_FillRect(_SURFACE_,NULL,SDL_MapRGB(_SURFACE_->format,0,0,0))
#define FLIP_WINDOW SDL_Flip(SDL_GetVideoSurface())


