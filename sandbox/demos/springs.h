#pragma once

#include<SDL3/SDL.h>
#include <vector>
#include <utils.h>
#include <iostream>

#include <demo.h>
#include <Brise/World.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

	class SpringsDemo : public Demo {

	public:
		SpringsDemo() { Init(); }
		~SpringsDemo() { Shutdown(); }

	private:

		float particleRadius = 25; // Particle radius in pixels for debug drawing
		Brise::World physicsWorld;
		std::unique_ptr<Brise::ParticleSpring> springGenerator;

	public:

		void PollEvent(AppContext* appstate, SDL_Event* event) override {
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

			Utils::DrawLine(appstate->renderer, particles[0].position, particles[1].position);
		}

	private:
		void Init() {
			Brise::Particle& p1 = physicsWorld.AddParticule({ -3, 3 }, 3, 0.999);
			Brise::Particle& p2 = physicsWorld.AddParticule({ -3, 0 }, 3, 0.999);
			p1.acceleration = { 0, 0 };
			
			springGenerator = std::make_unique<Brise::ParticleSpring>(&p1, 20, 3);
			physicsWorld.AddForceGenToRegistry(&p2, springGenerator.get());
		}
		void Shutdown() {}

	};
}