#pragma once

#include<SDL3/SDL.h>
#include <vector>
#include <utils.h>
#include <iostream>

#include <demo.h>
#include <Brise/World.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

	class ParticlesDemo : public Demo {

	public:
		ParticlesDemo() { Init(); }
		~ParticlesDemo() { Shutdown(); }

	private:
		
		float particleRadius = 25; // Particle radius in pixels for debug drawing
		Brise::World physicsWorld;

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

					physicsWorld.AddParticule(mousePos, 1, 0.999);
				}
			}
		}

		void Update(double deltaTime) override {
			physicsWorld.Step(deltaTime);
		}

		void Render(AppContext* appstate) override {
			SDL_SetRenderDrawColor(appstate->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

			const auto& particles = physicsWorld.GetParticles();
			
			for (auto& p : particles) {
				Utils::DrawCircle(
					appstate->renderer,
					p.position,
					particleRadius
				);

				Utils::DrawParticleInfos(appstate->renderer, p);
			}

			SDL_SetRenderScale(appstate->renderer, 1.5, 1.5);
			SDL_RenderDebugText(appstate->renderer, 14, 565, "Click anywhere to spawn particle");
			SDL_SetRenderScale(appstate->renderer, 1, 1);
		}

		const char* GetName() const override {
			return "Particles Demo";
		}
		
	private:
		void Init() {}
		void Shutdown() {}
		
	};
}