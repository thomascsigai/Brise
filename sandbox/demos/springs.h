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
		std::unique_ptr<Brise::ParticleSpring> springGenerator2;
		std::unique_ptr<Brise::AnchoredParticleSpring> anchoredSpring;
		std::unique_ptr<Brise::ParticleBungee> bungeeSpring;
		std::unique_ptr<Brise::ParticleBungee> bungeeSpring2;

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
			Utils::DrawLine(appstate->renderer, particles[2].position, {0, 3});
			Utils::DrawLine(appstate->renderer, particles[3].position, particles[4].position);
		}

		const char* GetName() const override {
			return "Springs Demo";
		}

	private:
		void Init() {
			Brise::Particle& p1 = physicsWorld.AddParticule({ -5, 3 }, 3, 0.9);
			Brise::Particle& p2 = physicsWorld.AddParticule({ -5, 0 }, 3, 0.9);
			p1.acceleration = { 0, 0 };
			p2.acceleration = { 0, 0 };
			
			springGenerator = std::make_unique<Brise::ParticleSpring>(&p1, 1, 5);
			springGenerator2 = std::make_unique<Brise::ParticleSpring>(&p2, 1, 5);
			physicsWorld.AddForceGenToRegistry(&p2, springGenerator.get());
			physicsWorld.AddForceGenToRegistry(&p1, springGenerator2.get());

			Brise::Particle& p3 = physicsWorld.AddParticule({ -2, 0 }, 3, 0.9);
			Brise::Vec2 anchor = { 0, 3 };
			anchoredSpring = std::make_unique<Brise::AnchoredParticleSpring>(anchor, 50, 3);
			physicsWorld.AddForceGenToRegistry(&p3, anchoredSpring.get());

			Brise::Particle& p4 = physicsWorld.AddParticule({ 5, 5 }, 3, 0.9);
			Brise::Particle& p5 = physicsWorld.AddParticule({ 5, -5 }, 3, 0.9);
			p4.acceleration = { 0, 0 };
			p5.acceleration = { 0, 0 };
			bungeeSpring = std::make_unique<Brise::ParticleBungee>(&p4, 1, 5);
			bungeeSpring2 = std::make_unique<Brise::ParticleBungee>(&p5, 1, 5);
			physicsWorld.AddForceGenToRegistry(&p5, bungeeSpring.get());
			physicsWorld.AddForceGenToRegistry(&p4, bungeeSpring2.get());
		}
		void Shutdown() {}

	};
}