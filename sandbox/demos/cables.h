#pragma once

#include <SDL3/SDL.h>
#include <demo.h>
#include <utils.h>
#include <Brise/World.h>
#include <Brise/PLinks.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

    class CableDemo : public Demo
    {
    private:
        Brise::World physicsWorld;

        Brise::Particle* p0;
        Brise::Particle* p1;
        Brise::Particle* p2;

        std::unique_ptr<Brise::ParticleCable> cable;
        std::unique_ptr<Brise::ParticleCable> cable2;

        float particleRadius = 25;

    public:

        CableDemo() { Init(); }

        const char* GetName() const override {
            return "Cable Demo";
        }

        void PollEvent(AppContext* appstate, SDL_Event* event) override {}

        void Update(double deltaTime) override
        {
            physicsWorld.Step(deltaTime);
        }

        void Render(AppContext* app) override
        {
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

            const auto& particles = physicsWorld.GetParticles();

            for (auto& p : particles)
            {
                Utils::DrawCircle(app->renderer, p.position, particleRadius);
                Utils::DrawParticleInfos(app->renderer, p);
            }

            // Draw cable
            Utils::DrawLine(app->renderer, p0->position, p1->position);
            Utils::DrawLine(app->renderer, p1->position, p2->position);
        }

    private:

        void Init()
        {
            p0 = &physicsWorld.AddParticule({ -2, 2 }, 1.0f, 0.99f);
            p0->SetInfiniteMass();

            p1 = &physicsWorld.AddParticule({ 2, 2 }, 1.0f, 0.99f);
            p2 = &physicsWorld.AddParticule({ 4, 2 }, 1.0f, 0.99f);

            p0->velocity = { 0, 0 };
            p1->velocity = { 0, 0 };

            cable = std::make_unique<Brise::ParticleCable>();
            cable2 = std::make_unique<Brise::ParticleCable>();

            cable->particle[0] = p0;
            cable->particle[1] = p1;
            cable->maxLength = 3.0f;
            cable->restitution = 0.5f;

            cable2->particle[0] = p1;
            cable2->particle[1] = p2;
            cable2->maxLength = 3.0f;
            cable2->restitution = 0.5f;

            physicsWorld.AddContactGenerator(cable.get());
            physicsWorld.AddContactGenerator(cable2.get());
        }
    };

}
