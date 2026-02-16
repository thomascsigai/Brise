#pragma once

#include<SDL3/SDL.h>
#include <vector>
#include <utils.h>
#include <iostream>

#include <demo.h>
#include <Brise/World.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

	class BuoyancyDemo : public Demo {

	public:
		BuoyancyDemo() { Init(); }
		~BuoyancyDemo() { Shutdown(); }

	private:

		float particleRadius = 25; // Particle radius in pixels for debug drawing
		Brise::World physicsWorld;
		std::unique_ptr<Brise::ParticleBuoyancy> buoyancyGenerator;
		std::unique_ptr<Brise::ParticleBuoyancy> buoyancyGenerator2;
		std::unique_ptr<Brise::ParticleBuoyancy> buoyancyGenerator3;

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

			Utils::DrawLine(appstate->renderer, { -13, 1}, {13, 1});
		}

		const char* GetName() const override {
			return "Buoyancy Demo";
		}

	private:
		void Init() {
			Brise::Particle& p1 = physicsWorld.AddParticule({ -5, 3 }, 0.5f, 0.6);
			buoyancyGenerator = std::make_unique<Brise::ParticleBuoyancy>(0.25f, 0.006f, 1);
			physicsWorld.AddForceGenToRegistry(&p1, buoyancyGenerator.get());

			Brise::Particle& p2 = physicsWorld.AddParticule({ 0, 3 }, 1.0f, 0.6);
			buoyancyGenerator2 = std::make_unique<Brise::ParticleBuoyancy>(0.25f, 0.02f, 1);
			physicsWorld.AddForceGenToRegistry(&p2, buoyancyGenerator2.get());
			
			Brise::Particle& p3 = physicsWorld.AddParticule({ 3, 3 }, 5.0f, 0.6);
			buoyancyGenerator3 = std::make_unique<Brise::ParticleBuoyancy>(0.25f, 0.01f, 1);
			physicsWorld.AddForceGenToRegistry(&p3, buoyancyGenerator3.get());
		}
		void Shutdown() {}

	};
}