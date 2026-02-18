#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <utils.h>
#include <iostream>

#include <demo.h>
#include <Brise/World.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

	enum CollisionTest {
		simpleCollision,
		nonLinearContact,
		differentSpeed,
		differentMasses,
		zeroRestitution,
		midRestitution,
		infiniteMass,
		interpenetration,
		stabilization
	};

	class CollisionDemo : public Demo {

	public:
		CollisionDemo() { Init(); }
		~CollisionDemo() { Shutdown(); }

	private:

		float particleRadius = 25; // Particle radius in pixels for debug drawing
		Brise::World physicsWorld;
		
		Brise::Particle* p0;
		Brise::Particle* p1;

		CollisionTest currentTest = CollisionTest::simpleCollision;

	public:

		void PollEvent(AppContext* appstate, SDL_Event* event) override {
            if (event->type == SDL_EVENT_KEY_DOWN)
            {
                switch (event->key.scancode)
                {
                case SDL_SCANCODE_Q:
                    SetupTest(simpleCollision);
                    break;

                case SDL_SCANCODE_W:
                    SetupTest(nonLinearContact);
                    break;

                case SDL_SCANCODE_E:
                    SetupTest(differentSpeed);
                    break;

                case SDL_SCANCODE_R:
                    SetupTest(differentMasses);
                    break;

                case SDL_SCANCODE_T:
                    SetupTest(zeroRestitution);
                    break;

                case SDL_SCANCODE_Y:
                    SetupTest(midRestitution);
                    break;

                case SDL_SCANCODE_U:
                    SetupTest(infiniteMass);
                    break;

                case SDL_SCANCODE_I:
                    SetupTest(interpenetration);
                    break;

                case SDL_SCANCODE_O:
                    SetupTest(stabilization);
                    break;

                default:
                    break;
                }
            }
		}

		void Update(float deltaTime) override {
            physicsWorld.Update(deltaTime);

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

                switch (currentTest) {
                case zeroRestitution: contact.restitution = 0.0f; break;
                case midRestitution:  contact.restitution = 0.5f; break;
                default:              contact.restitution = 1.0f; break;
                }

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
            SDL_SetRenderScale(appstate->renderer, 4, 4);
            SDL_RenderDebugText(
                appstate->renderer,
                5, 200,
                GetTestName()
            );
            SDL_SetRenderScale(appstate->renderer, 1.5, 1.5);
            SDL_RenderDebugText(appstate->renderer, 14, 565, "Press Q, W, E, R, T... to change test");
            SDL_SetRenderScale(appstate->renderer, 1, 1);
		}

        const char* GetName() const override {
            return "Collision Demo";
        }

	private:
		void Init() {

			switch (currentTest) {

            case simpleCollision:
                p0 = &physicsWorld.AddParticule({ -3, 0 }, 3, 0.99f);
                p1 = &physicsWorld.AddParticule({ 3, 0 }, 3, 0.99f);
                p0->velocity = { 2, 0 };
                p1->velocity = { -2, 0 };
                break;


            case nonLinearContact:
                p0 = &physicsWorld.AddParticule({ -3, -1 }, 3, 0.99f);
                p1 = &physicsWorld.AddParticule({ 3, 0 }, 3, 0.99f);
                p0->velocity = { 2, 1 };
                p1->velocity = { -2, 0 };
                break;


            case differentSpeed:
                p0 = &physicsWorld.AddParticule({ -3, 0 }, 3, 0.99f);
                p1 = &physicsWorld.AddParticule({ 3, 0 }, 3, 0.99f);
                p0->velocity = { 5, 0 };
                p1->velocity = { -1, 0 };
                break;


            case differentMasses:
                p0 = &physicsWorld.AddParticule({ -3, 0 }, 1, 0.99f);
                p1 = &physicsWorld.AddParticule({ 3, 0 }, 10, 0.99f);
                p0->velocity = { 5, 0 };
                p1->velocity = { 0, 0 };
                break;


            case zeroRestitution:
                p0 = &physicsWorld.AddParticule({ -3, 0 }, 3, 0.99f);
                p1 = &physicsWorld.AddParticule({ 3, 0 }, 3, 0.99f);
                p0->velocity = { 3, 0 };
                p1->velocity = { -3, 0 };
                break;


            case midRestitution:
                p0 = &physicsWorld.AddParticule({ -3, 0 }, 3, 0.99f);
                p1 = &physicsWorld.AddParticule({ 3, 0 }, 3, 0.99f);
                p0->velocity = { 4, 0 };
                p1->velocity = { -4, 0 };
                break;


            case infiniteMass:
                p0 = &physicsWorld.AddParticule({ -3, 0 }, 3, 0.99f);
                p1 = &physicsWorld.AddParticule({ 3, 0 }, 1, 0.99f);
                p1->SetInfiniteMass();
                p0->velocity = { 5, 0 };
                p1->velocity = { 0, 0 };
                break;


            case interpenetration:
                p0 = &physicsWorld.AddParticule({ -0.2f, 0 }, 3, 0.99f);
                p1 = &physicsWorld.AddParticule({ 0.2f, 0 }, 3, 0.99f);
                p0->velocity = { 0, 0 };
                p1->velocity = { 0, 0 };
                break;


            case stabilization:
                p0 = &physicsWorld.AddParticule({ -1, 0 }, 3, 0.99f);
                p1 = &physicsWorld.AddParticule({ 1, 0 }, 3, 0.99f);
                p0->acceleration = { 0, 0 };
                p1->acceleration = { 0, 0 };
                p0->velocity = { 0.5f, 0 };
                p1->velocity = { -0.5f, 0 };
                break;
			}

			p0->acceleration = { 0, 0 };
			p1->acceleration = { 0, 0 };

		}

		void Shutdown() {}

		void SetupTest(CollisionTest test) {
			physicsWorld = Brise::World();   
			currentTest = test;
			Init();                          
		}

        const char* GetTestName() const
        {
            switch (currentTest)
            {
            case simpleCollision:     return "Simple Collision";
            case nonLinearContact:    return "Non-linear Contact";
            case differentSpeed:      return "Different Speed";
            case differentMasses:     return "Different Masses";
            case zeroRestitution:     return "Zero Restitution";
            case midRestitution:      return "Mid Restitution";
            case infiniteMass:        return "Infinite Mass";
            case interpenetration:    return "Interpenetration";
            case stabilization:       return "Stabilization";
            default:                  return "Unknown Test";
            }
        }

	};
}