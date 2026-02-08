#pragma once

#include<SDL3/SDL.h>
#include <vector>
#include <utils.h>
#include <iostream>

#include <demo.h>
#include <Brise/particle.h>
#include <Brise/Vec2.h>
#include <Brise/PForceGen.h>

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
		Brise::ParticleForceRegistry forceRegistry = Brise::ParticleForceRegistry();

	public:

		void PollEvent(AppContext* appstate, SDL_Event* event) override {
			if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				float x = event->button.x;
				float y = event->button.y;

				if (event->button.button == SDL_BUTTON_LEFT) {
					Brise::Vec2 mousePos = Utils::ScreenToWorld(
						appstate->renderer, 
						{ x, y }
					);

					particles.push_back(Brise::Particle(mousePos, 1, 0.999f));
					particles.back().acceleration = { 0, -9.81 };
				}
			}
		}

		void Update(double deltaTime) override {
			forceRegistry.UpdateForces(deltaTime);
			
			for (auto& p : particles) {
				p.Integrate(deltaTime);
			}
		}

		void Render(AppContext* appstate) override {
			SDL_SetRenderDrawColor(appstate->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			
			for (auto& p : particles) {
				Utils::DrawCircle(
					appstate->renderer,
					p.position,
					particleRadius
				);

				DrawParticleInfos(appstate->renderer, p);
			}
		}
		
	private:
		void Init() {
			particles.reserve(MAX_PARTICLES);
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
	};
}