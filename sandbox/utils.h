#pragma once

#include <SDL3/SDL.h>

#include <Brise/Vec2.h>

namespace Utils {
	constexpr int PIXELS_PER_METER = 75.0f;

	// BRISE AXIS :
	// ORIGIN AT THE CENTER OF THE SCREEN
	// RIGHT HANDED - Y UP

	// Convert World Coordinates (meters) to screen coordinates (pixels).
	inline Brise::Vec2 WorldToScreenPosition(
		SDL_Renderer* renderer,
		const Brise::Vec2& worldPos
	)
	{
		int screenWidth, screenHeight;
		SDL_GetWindowSize(
			SDL_GetRenderWindow(renderer),
			&screenWidth,
			&screenHeight
		);

		Brise::Vec2 pos;

		pos.x = PIXELS_PER_METER * worldPos.x + screenWidth / 2;
		pos.y = screenHeight / 2 - PIXELS_PER_METER * worldPos.y;

		return pos;
	}

	// Convert Screen Coordinates (pixels) to World coordinates (meters).
	inline Brise::Vec2 ScreenToWorld(
		SDL_Renderer* renderer,
		const Brise::Vec2& screenPos
	) {

		int screenWidth, screenHeight;
		SDL_GetWindowSize(
			SDL_GetRenderWindow(renderer),
			&screenWidth,
			&screenHeight
		);

		float METERS_PER_PIXEL = 1.0f / PIXELS_PER_METER;

		Brise::Vec2 pos;
		pos.x = screenPos.x * METERS_PER_PIXEL - screenWidth / 2 * METERS_PER_PIXEL;
		pos.y = (screenHeight / 2 - screenPos.y) * METERS_PER_PIXEL;

		return pos;
	}

	void DrawCircle(
		SDL_Renderer* renderer, 
		Brise::Vec2 pos, 
		int32_t radius
	)
	{

		Brise::Vec2 screenPos = WorldToScreenPosition(renderer, pos);

		const int32_t diameter = (radius * 2);

		int32_t x = (radius - 1);
		int32_t y = 0;
		int32_t tx = 1;
		int32_t ty = 1;
		int32_t error = (tx - diameter);

		while (x >= y)
		{
			//  Each of the following renders an octant of the circle
			SDL_RenderPoint(renderer, screenPos.x + x, screenPos.y - y);
			SDL_RenderPoint(renderer, screenPos.x + x, screenPos.y + y);
			SDL_RenderPoint(renderer, screenPos.x - x, screenPos.y - y);
			SDL_RenderPoint(renderer, screenPos.x - x, screenPos.y + y);
			SDL_RenderPoint(renderer, screenPos.x + y, screenPos.y - x);
			SDL_RenderPoint(renderer, screenPos.x + y, screenPos.y + x);
			SDL_RenderPoint(renderer, screenPos.x - y, screenPos.y - x);
			SDL_RenderPoint(renderer, screenPos.x - y, screenPos.y + x);

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

	void DrawFilledCircle(
		SDL_Renderer* renderer, 
		Brise::Vec2 pos, 
		int radius, 
		SDL_Color color
	)
	{
		Brise::Vec2 screenPos = WorldToScreenPosition(renderer, pos);

		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		for (int w = 0; w < radius * 2; w++)
		{
			for (int h = 0; h < radius * 2; h++)
			{
				int dx = radius - w; // horizontal offset
				int dy = radius - h; // vertical offset
				if ((dx * dx + dy * dy) <= (radius * radius))
				{
					SDL_RenderPoint(renderer, screenPos.x + dx, screenPos.y + dy);
				}
			}
		}
	}

	void DrawLine(
		SDL_Renderer* renderer, 
		const Brise::Vec2& pos1, const Brise::Vec2& pos2
	)
	{
		Brise::Vec2 screenPos1 = Brise::Vec2(pos1);
		screenPos1 = WorldToScreenPosition(renderer, screenPos1);

		Brise::Vec2 screenPos2 = Brise::Vec2(pos2);
		screenPos2 = WorldToScreenPosition(renderer, screenPos2);

		SDL_RenderLine(renderer, screenPos1.x, screenPos1.y, screenPos2.x, screenPos2.y);
	}

	void DrawDebugAxis(void* appstate)
	{
		auto* app = (AppContext*)appstate;
		int screenWidth, screenHeight;
		SDL_GetWindowSize(app->window, &screenWidth, &screenHeight);
		
		// Draw other lines
		SDL_SetRenderDrawColor(app->renderer, 80, 80, 80, 100);
		for (int i = 0; i < 2000; i++)
		{
			DrawLine(
				app->renderer, 
				{ -1000, 1000 - (float)i }, 
				{ 1000, 1000 - (float)i }
				);
			DrawLine(
				app->renderer,
				{ -1000 + (float)i, -1000 },
				{ -1000 + (float)i, 1000 }
			);
		}

		// Draw x axis
		SDL_SetRenderDrawColor(app->renderer, 155, 0, 0, 150);
		DrawLine(
			app->renderer,
			{ -1000, 0 }, { 1000, 0 }
		);

		// Draw y axis
		SDL_SetRenderDrawColor(app->renderer, 0, 0, 155, 150);
		DrawLine(
			app->renderer,
			{ 0, 1000 }, { 0, -1000 }
		);

		// Reset drawing color to white
		SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
	}

	void DrawParticleInfos(SDL_Renderer* renderer, const Brise::Particle& p) {
		char text[256];
		Brise::Vec2 pScreenPos = Utils::WorldToScreenPosition(renderer, p.position);

		// Draw position under particle
		SDL_snprintf(
			text,
			sizeof(text),
			"Pos (%.2f, y:%.2f)",
			p.position.x,
			p.position.y
		);

		SDL_RenderDebugText(renderer, pScreenPos.x - 65, pScreenPos.y + 40, text);

		// Draw velocity under particle
		SDL_snprintf(
			text,
			sizeof(text),
			"Vel (%.2f, y:%.2f)",
			p.velocity.x,
			p.velocity.y
		);

		SDL_RenderDebugText(renderer, pScreenPos.x - 65, pScreenPos.y + 50, text);

		// Draw force accumulation under particle
		SDL_snprintf(
			text,
			sizeof(text),
			"Acc (%.2f, y:%.2f)",
			p.acceleration.x,
			p.acceleration.y
		);

		SDL_RenderDebugText(renderer, pScreenPos.x - 65, pScreenPos.y + 60, text);
	}

	/// <summary>
		/// Tells if the mouse position in inside a particle area.
		/// </summary>
		/// <param name="mouseX">Mouse X axis screen pos (in pixels)</param>
		/// <param name="mouseY">Mouse Y axis screen pos (in pixels)</param>
		/// <returns>True if mouse is over particle, false otherwise.</returns>
	bool MouseOverParticle(
		SDL_Renderer* renderer,
		float mouseX, float mouseY,
		const Brise::Particle& p
	) {
		Brise::Vec2 mousePos = Utils::ScreenToWorld(renderer, { mouseX, mouseY });

		if (Brise::DistanceSquared(mousePos, p.position) < powf(particleRadius / Utils::PIXELS_PER_METER, 2)) {
			return true;
		}

		return false;
	}
}
