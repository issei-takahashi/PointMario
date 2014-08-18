#ifndef SDL_DRAW_H
#define SDL_DRAW_H

#include <SDL.h>

#ifdef __cplusplus
extern "C"{
#endif

	void SDL_DrawPixel(SDL_Surface *screen,
		int x, int y,
		Uint8 R, Uint8 G, Uint8 B);
	void SDL_DrawLine(SDL_Surface *screen,
		int x1, int y1,
		int x2, int y2,
		Uint8 R, Uint8 G, Uint8 B);
	void DrawBox(SDL_Surface *screen,
		int x1, int y1,
		int x2, int y2,
		Uint8 R, Uint8 G, Uint8 B);
	void SDL_DrawCircle(SDL_Surface *screen,
		int x1, int y1,
		int x2, int y2,
		Uint8 R, Uint8 G, Uint8 B);
	void SDL_DrawFullCircle(SDL_Surface *screen,
		int x1, int y1,
		int x2, int y2,
		Uint8 R, Uint8 G, Uint8 B);
	void SDL_DrawPaint(SDL_Surface *screen,
		int x, int y,
		Uint8 R, Uint8 G, Uint8 B);

#ifdef __cplusplus
}
#endif

#endif