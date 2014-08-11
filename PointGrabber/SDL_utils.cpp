#include <SDL.h>
#include <SDL_endian.h>	/* エンディアンに依存しない 24bpp モードの時に使われる */
#include "SDL_utils.h"
#include <stdlib.h>

typedef struct point_st{
	int x, y;
} point_t;

static void SDL_PutPixel(SDL_Surface *screen,
	int x, int y,
	Uint32 color){
		switch(screen->format->BytesPerPixel){
		case 1:{
			Uint8 *bufp;

			bufp = (Uint8 *)screen->pixels + y * screen->pitch + x;
			*bufp = color;
			break;
			   }

		case 2:{
			Uint16 *bufp;

			bufp = (Uint16 *)screen->pixels + y * screen->pitch / 2 + x;
			*bufp = color;
			break;
			   }

		case 3:{
			Uint8 *bufp;

			bufp = (Uint8 *)screen->pixels + y * screen->pitch + x * 3;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			bufp[0] = color;
			bufp[1] = color >> 8;
			bufp[2] = color >> 16;
#else
			bufp[2] = color;
			bufp[1] = color >> 8;
			bufp[0] = color >> 16;
#endif
			break;
			   }

		case 4:{
			Uint32 *bufp;

			bufp = (Uint32 *)screen->pixels + y * screen->pitch / 4 + x;
			*bufp = color;
			break;
			   }
		}
}

static Uint32 SDL_GetPixel(SDL_Surface *screen,
	int x, int y){
		Uint32 color = 0;
		switch(screen->format->BytesPerPixel){
		case 1:{
			Uint8 *bufp;

			bufp = (Uint8 *)screen->pixels + y * screen->pitch + x;
			color = *bufp;
			break;
			   }

		case 2:{
			Uint16 *bufp;

			bufp = (Uint16 *)screen->pixels + y * screen->pitch / 2 + x;
			color = *bufp;
			break;
			   }

		case 3:{
			Uint8 *bufp;

			bufp = (Uint8 *)screen->pixels + y * screen->pitch + x * 3;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			color = bufp[0] | (bufp[1] << 8) | (bufp[2] << 16);
#else
			color = bufp[2] | (bufp[1] << 8) | (bufp[0] << 16);
#endif
			break;
			   }

		case 4:{
			Uint32 *bufp;

			bufp = (Uint32 *)screen->pixels + y * screen->pitch / 4 + x;
			color = *bufp;
			break;
			   }
		}
		return color;
}

void SDL_DrawPixel(SDL_Surface *screen,
	int x, int y,
	Uint8 R, Uint8 G, Uint8 B){
		Uint32 color = SDL_MapRGB(screen->format, R, G, B);

		if(SDL_MUSTLOCK(screen)){
			if(SDL_LockSurface(screen) < 0){
				return;
			}
		}

		SDL_PutPixel(screen, x, y, color);

		if(SDL_MUSTLOCK(screen)){
			SDL_UnlockSurface(screen);
		}
}

void SDL_DrawLine(SDL_Surface *screen,
	int x1, int y1,
	int x2, int y2,
	Uint8 R, Uint8 G, Uint8 B){
		Uint32 color = SDL_MapRGB(screen->format, R, G, B);

		int dx, dy, dx2, dy2;
		int sx, sy;
		int i, l;
		int e;

		if( (x1 < 0) | (screen->w <= x1) |
			(x2 < 0) | (screen->w <= x2) |
			(y1 < 0) | (screen->h <= y1) |
			(y2 < 0) | (screen->h <= y2))
			return;

		if(SDL_MUSTLOCK(screen)){
			if(SDL_LockSurface(screen) < 0){
				return;
			}
		}

		dx = x2 - x1 < 0 ? x1 - x2 : x2 - x1;
		dy = y2 - y1 < 0 ? y1 - y2 : y2 - y1;
		sx = x2 - x1 < 0 ? -1 : 1;
		sy = y2 - y1 < 0 ? -1 : 1;
		dx2 = dx * 2;
		dy2 = dy * 2;

		if(x1 == x2){
			if(y1 < y2){
				for(i = y1;i < y2;i++){
					SDL_PutPixel(screen, x1, i, color);
				}
			}
			else{
				for(i = y2;i < y1;i++){
					SDL_PutPixel(screen, x1, i, color);
				}
			}
		}
		else if(y1 == y2){
			if(x1 < x2){
				for(i = x1;i < x2;i++){
					SDL_PutPixel(screen, i, y1, color);
				}
			}
			else{
				for(i = x2;i < x1;i++){
					SDL_PutPixel(screen, i, y1, color);
				}
			}
		}
		else if(dx >= dy){
			e = -dx;
			l = (dx + 1) / 2;
			for(i = 0;i < l;i++){
				SDL_PutPixel(screen, x1, y1, color);
				SDL_PutPixel(screen, x2, y2, color);
				x1 += sx;
				x2 -= sx;
				e += dy2;
				if(e >= 0){
					y1 += sy;
					y2 -= sy;
					e -= dx2;
				}
			}
			if(dx % 2 == 0)
				SDL_PutPixel(screen, x1, y1, color);
		}
		else{
			e = -dy;
			l = (dy + 1) / 2;
			for(i = 0;i < l;i++){
				SDL_PutPixel(screen, x1, y1, color);
				SDL_PutPixel(screen, x2, y2, color);
				y1 += sy;
				y2 -= sy;
				e += dx2;
				if(e >= 0){
					x1 += sx;
					x2 -= sx;
					e -= dy2;
				}
			}
			if(dy % 2 == 0)
				SDL_PutPixel(screen, x1, y1, color);
		}

		if(SDL_MUSTLOCK(screen)){
			SDL_UnlockSurface(screen);
		}
}

void DrawBox(SDL_Surface *screen,
	int x1, int y1,
	int x2, int y2,
	Uint8 R, Uint8 G, Uint8 B){
		SDL_DrawLine(screen, x1, y1, x2, y1, R, G, B);
		SDL_DrawLine(screen, x2, y1, x2, y2, R, G, B);
		SDL_DrawLine(screen, x2, y2, x1, y2, R, G, B);
		SDL_DrawLine(screen, x1, y2, x1, y1, R, G, B);
}

void SDL_DrawCircle(SDL_Surface *screen,
	int x1, int y1,
	int x2, int y2,
	Uint8 R, Uint8 G, Uint8 B){
		Uint32 color = SDL_MapRGB(screen->format, R, G, B);

		int r;
		int dx, dy;
		int xa, ya, xb, yb;
		int f;

		if( (x1 < 0) | (screen->w <= x1) |
			(x2 < 0) | (screen->w <= x2) |
			(y1 < 0) | (screen->h <= y1) |
			(y2 < 0) | (screen->h <= y2))
			return;
		if(x1 == x2 && y1 == y2)
			return;

		if(SDL_MUSTLOCK(screen)){
			if(SDL_LockSurface(screen) < 0){
				return;
			}
		}

		if(x1 > x2){
			int tmp = x2;
			x2 = x1;
			x1 = tmp;
		}
		if(y1 > y2){
			int tmp = y2;
			y2 = y1;
			y1 = tmp;
		}

		dx = x2 - x1 < 0 ? x1 - x2 : x2 - x1;
		dy = y2 - y1 < 0 ? y1 - y2 : y2 - y1;
		if(dx >= dy){
			r = dx / 2;
			xa = r;
			ya = 0;
			f = 3 - 2 * r;
			x1 += dx / 2;
			y1 += dy / 2;
			x2 = x1 + dx % 2;
			y2 = y1 + dy % 2;
			while(xa >= ya){
				xb = (xa * dy) / dx;
				yb = (ya * dy) / dx;
				SDL_PutPixel(screen, xa + x2,-yb + y1, color); /*A*/
				SDL_PutPixel(screen, ya + x2,-xb + y1, color); /*B*/
				SDL_PutPixel(screen,-ya + x1,-xb + y1, color); /*C*/
				SDL_PutPixel(screen,-xa + x1,-yb + y1, color); /*D*/
				SDL_PutPixel(screen,-xa + x1, yb + y2, color); /*E*/
				SDL_PutPixel(screen,-ya + x1, xb + y2, color); /*F*/
				SDL_PutPixel(screen, ya + x2, xb + y2, color); /*G*/
				SDL_PutPixel(screen, xa + x2, yb + y2, color); /*H*/
				if(f >= 0){
					xa--;
					f -= 4 * xa;
				}
				ya++;
				f += 4 * ya + 2;
			}
		}
		else{
			r = dy / 2;
			ya = r;
			xa = 0;
			f = 3 - 2 * r;
			x1 += dx / 2;
			y1 += dy / 2;
			x2 = x1 + dx % 2;
			y2 = y1 + dy % 2;
			while(ya >= xa){
				xb = (xa * dx) / dy;
				yb = (ya * dx) / dy;
				SDL_PutPixel(screen, xb + x2,-ya + y1, color); /*A*/
				SDL_PutPixel(screen, yb + x2,-xa + y1, color); /*B*/
				SDL_PutPixel(screen,-yb + x1,-xa + y1, color); /*C*/
				SDL_PutPixel(screen,-xb + x1,-ya + y1, color); /*D*/
				SDL_PutPixel(screen,-xb + x1, ya + y2, color); /*E*/
				SDL_PutPixel(screen,-yb + x1, xa + y2, color); /*F*/
				SDL_PutPixel(screen, yb + x2, xa + y2, color); /*G*/
				SDL_PutPixel(screen, xb + x2, ya + y2, color); /*H*/
				if(f >= 0){
					ya--;
					f -= 4 * ya;
				}
				xa++;
				f += 4 * xa + 2;
			}
		}

		if(SDL_MUSTLOCK(screen)){
			SDL_UnlockSurface(screen);
		}
}

void SDL_DrawPaint(SDL_Surface *screen,
	int x, int y,
	Uint8 R, Uint8 G, Uint8 B){
		Uint32 color = SDL_MapRGB(screen->format, R, G, B);

		point_t *buffer;
		int pbuf;
		Uint32 bcol;
		int i;
		int bx1,bx2,by;

		if( (x < 0) | (screen->w <= x) |
			(y < 0) | (screen->h <= y))
			return;

		buffer = (point_t*)malloc(sizeof(point_t) * (screen->w + 1));
		if(buffer == NULL)
			return;

		if(SDL_MUSTLOCK(screen)){
			if(SDL_LockSurface(screen) < 0){
				free(buffer);
				return;
			}
		}

		bcol = SDL_GetPixel(screen, x, y);
		if(bcol == color){
			if(SDL_MUSTLOCK(screen)){
				SDL_UnlockSurface(screen);
			}
			free(buffer);
			return;
		}
		buffer[0].x = x;
		buffer[0].y = y;
		pbuf = 1;
		while(pbuf){
			pbuf--;
			bx1 = bx2 = buffer[pbuf].x;
			by = buffer[pbuf].y;
			while(bx1 >= 0 && SDL_GetPixel(screen, bx1, by) == bcol)
				bx1--;
			while(bx2 < screen->w && SDL_GetPixel(screen, bx2, by) == bcol)
				bx2++;
			bx1++;
			bx2--;
			for(i = bx1;i <= bx2;i++)
				SDL_PutPixel(screen, i, by, color);
			if(by > 0){
				i = bx1;
				by--;
				if(SDL_GetPixel(screen, i, by) == bcol){
					while(i >= 0 && SDL_GetPixel(screen, i, by) == bcol)
						i--;
					i++;
				}
				else
					while(i <= bx2 && SDL_GetPixel(screen, i, by) != bcol)
						i++;
				while(i <= bx2){
					buffer[pbuf].x = i;
					buffer[pbuf].y = by;
					pbuf++;
					while(i < screen->w && SDL_GetPixel(screen, i, by) == bcol)
						i++;
					while(i <= bx2 && SDL_GetPixel(screen, i, by) != bcol)
						i++;
				}
			}
			else
				by--;
			if(by + 2 < screen->h){
				i = bx1;
				by += 2;
				if(SDL_GetPixel(screen, i, by) == bcol){
					while(i >= 0 && SDL_GetPixel(screen, i, by) == bcol)
						i--;
					i++;
				}
				else
					while(i <= bx2 && SDL_GetPixel(screen, i, by) != bcol)
						i++;
				while(i <= bx2){
					buffer[pbuf].x = i;
					buffer[pbuf].y = by;
					pbuf++;
					while(i < screen->w && SDL_GetPixel(screen, i, by) == bcol)
						i++;
					while( i <= bx2 && SDL_GetPixel(screen, i, by) != bcol)
						i++;
				}
			}
		}

		if(SDL_MUSTLOCK(screen)){
			SDL_UnlockSurface(screen);
		}
		free(buffer);
}