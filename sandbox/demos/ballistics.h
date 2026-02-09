#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <utils.h>
#include <iostream>

#include <demo.h>
#include <Brise/World.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

	class BallisticsDemo : public Demo {

	public:
		BallisticsDemo() { Init(); }
		~BallisticsDemo() { Shutdown(); }

	private:

		float particleRadius = 25; // Particle radius in pixels for debug drawing
		Brise::World physicsWorld;

		enum BulletType {
			Pistol,
			Artillery,
			Fireball,
			Laser
		};

	public:

		void PollEvent(AppContext* appstate, SDL_Event* event) override {
			if (event->type == SDL_EVENT_KEY_DOWN) {
				if (event->key.scancode == SDL_SCANCODE_Q) {
					SpawnBullet(BulletType::Pistol);
				}
				else if (event->key.scancode == SDL_SCANCODE_W) {
					SpawnBullet(BulletType::Artillery);
				}
				else if (event->key.scancode == SDL_SCANCODE_E) {
					SpawnBullet(BulletType::Fireball);
				}
				else if (event->key.scancode == SDL_SCANCODE_R) {
					SpawnBullet(BulletType::Laser);
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
		}

	private:
		void Init() {}
		void Shutdown() {}

		void SpawnBullet(BulletType type) {
			Brise::Vec2 origin = { -10, 0 };
			
			Brise::Particle& p = physicsWorld.AddParticule(origin, 1, 0.999);

			switch (type) {

			case Pistol:
				p.velocity = { 35, 0 }; // 35 m/s
				p.acceleration = { 0, -1 };
				p.SetMass(2);
				break;

			case Artillery:
				p.velocity = { 30, 40 }; // 50 m/s
				p.acceleration = { 0, -20 };
				p.SetMass(200);
				break;

			case Fireball:
				p.velocity = { 10, 0 }; // 10 m/s
				p.acceleration = { 0, 0.6 };
				p.SetMass(1);
				p.SetDamping(0.7);
				break;

			case Laser:
				p.velocity = { 100, 0 }; // 100 m/s
				p.acceleration = { 0, 0 };
				p.SetMass(0.1);
				break;
			}
		}

	};
}