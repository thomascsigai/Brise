#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <utils.h>
#include <iostream>

#include <demo.h>
#include <Brise/World.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

	class CollisionDemo : public Demo {

	public:
		CollisionDemo() { Init(); }
		~CollisionDemo() { Shutdown(); }

	private:

		float particleRadius = 25; // Particle radius in pixels for debug drawing
		Brise::World physicsWorld;
		Brise::Particle* p0;
		Brise::Particle* p1;

	public:

		void PollEvent(AppContext* appstate, SDL_Event* event) override {
		}

		void Update(double deltaTime) override {
			physicsWorld.Step(deltaTime);

			Brise::World::ParticleContainer particles = physicsWorld.GetParticles();
			float distanceP0P1 = Brise::Magnitude(p0->position - p1->position);

			if (distanceP0P1 < 0.7) {
				Brise::ParticleContact contact;

				contact.particle[0] = p0;
				contact.particle[1] = p1;

				Brise::Vec2 normal = Normalize(p0->position - p1->position);
				contact.contactNormal = normal;

				float penetration = 0.7f - distanceP0P1;
				contact.penetration = penetration;

				contact.restitution = 1.0f;

				contact.Resolve(deltaTime);
			}
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
		}

	private:
		void Init() {
			p0 = &physicsWorld.AddParticule({ -3, 0.3 }, 3, 0.99);
			p1 = &physicsWorld.AddParticule({ 3, 0 }, 3, 0.99);
			p0->acceleration = { 0, 0 };
			p1->acceleration = { 0, 0 };
			p0->velocity = { 2, 0 };
			p1->velocity = { -5, 0 };
		}
		void Shutdown() {}

	};
}