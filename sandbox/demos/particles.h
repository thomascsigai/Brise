#pragma once

#include<SDL3/SDL.h>
#include <vector>
#include <utils.h>
#include <iostream>

#include <demo.h>
#include <Brise/particle.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

	constexpr int MAX_PARTICLES = 50;
	
	class ParticlesDemo : public Demo {

	public:
		ParticlesDemo() { Init(); }
		~ParticlesDemo() { Shutdown(); }

	// Attributes
	private:
		std::vector<Brise::Particle> particles;
		float particleRadius = 25; // Particle radius in pixels for debug drawing
		int indexSelectedParticle = -1;

	public:

		void PollEvent(AppContext* appstate, SDL_Event* event) override {
			if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				float x = event->button.x;
				float y = event->button.y;

				if (event->button.button == SDL_BUTTON_LEFT) {
					for (int i = 0; i < particles.size(); i++) {
						if (MouseOverParticle(appstate->renderer, x, y, particles[i])) {
							indexSelectedParticle = i;
						}
					}
				}
			}
		}

		void Update(double deltaTime) override {
			for (auto& p : particles) {
				p.Integrate(deltaTime);
			}
		}

		void Render(AppContext* appstate) override {
			SDL_SetRenderDrawColor(appstate->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			
			for (const auto& p : particles) {
				Utils::DrawCircle(
					appstate->renderer,
					p.position,
					particleRadius
				);
			}

			if (indexSelectedParticle > -1) {
				char text[256];
				SDL_snprintf(
					text,
					sizeof(text),
					"Selected particle -> x: %.2f y: %.2f",
					particles[indexSelectedParticle].position.x,
					particles[indexSelectedParticle].position.y
				);

				SDL_RenderDebugText(appstate->renderer, 10, 30, text);
			}
		}
		
	private:
		void Init() {
			particles.reserve(MAX_PARTICLES);

			particles.push_back(Brise::Particle({ 0, 0 }, 1, 0.99f));
			particles[0].acceleration = { 1, 1 };
			
			particles.push_back(Brise::Particle({ -5, 5 }, 1, 0.99f));
			particles[1].acceleration = { -2, -1 };
			
			particles.push_back(Brise::Particle({ 4, 2 }, 1, 0.99f));
			particles[2].acceleration = { 3, 1 };
		}

		void Shutdown() {

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
	};
}