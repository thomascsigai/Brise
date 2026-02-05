#pragma once

#include <SDL3/SDL.h>

namespace Utils {
	void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
	{
		const int32_t diameter = (radius * 2);

		int32_t x = (radius - 1);
		int32_t y = 0;
		int32_t tx = 1;
		int32_t ty = 1;
		int32_t error = (tx - diameter);

		while (x >= y)
		{
			//  Each of the following renders an octant of the circle
			SDL_RenderPoint(renderer, centreX + x, centreY - y);
			SDL_RenderPoint(renderer, centreX + x, centreY + y);
			SDL_RenderPoint(renderer, centreX - x, centreY - y);
			SDL_RenderPoint(renderer, centreX - x, centreY + y);
			SDL_RenderPoint(renderer, centreX + y, centreY - x);
			SDL_RenderPoint(renderer, centreX + y, centreY + x);
			SDL_RenderPoint(renderer, centreX - y, centreY - x);
			SDL_RenderPoint(renderer, centreX - y, centreY + x);

			if (error <= 0)
			{
				++y;
				error += ty;
				ty += 2;
			}

			if (error > 0)
			{
				--x;
				tx += 2;
				error += (tx - diameter);
			}
		}
	}

	void DrawFilledCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		for (int w = 0; w < radius * 2; w++)
		{
			for (int h = 0; h < radius * 2; h++)
			{
				int dx = radius - w; // horizontal offset
				int dy = radius - h; // vertical offset
				if ((dx * dx + dy * dy) <= (radius * radius))
				{
					SDL_RenderPoint(renderer, x + dx, y + dy);
				}
			}
		}
	}
}
