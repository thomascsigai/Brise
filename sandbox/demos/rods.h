#pragma once

#include <SDL3/SDL.h>
#include <demo.h>
#include <utils.h>
#include <Brise/World.h>
#include <Brise/PLinks.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

    class RodDemo : public Demo
    {
    private:
        Brise::World physicsWorld;

        std::vector<std::unique_ptr<Brise::ParticleRod>> rods;

        float particleRadius = 25;

    public:

        RodDemo() { Init(); }

        const char* GetName() const override {
            return "Rod Demo";
        }

        void PollEvent(AppContext* appstate, SDL_Event* event) override {}

        void Update(float deltaTime) override
        {
            physicsWorld.Update(deltaTime);
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

            // Draw rods
            for (const auto& rod : rods)
            {
                Utils::DrawLine(
                    app->renderer,
                    rod->particle[0]->position,
                    rod->particle[1]->position
                );
            }
        }

    private:

        void Init()
        {
            {
                auto* p0 = &physicsWorld.AddParticule({ -6, 3 }, 1.0f, 0.99f);
                auto* p1 = &physicsWorld.AddParticule({ -7, 0 }, 1.0f, 0.99f);
                p0->acceleration = { 0, 0 };
                p1->acceleration = { 0, 0 };
                p1->velocity = { 0, -1 };

                auto rod = std::make_unique<Brise::ParticleRod>();
                rod->particle[0] = p0;
                rod->particle[1] = p1;
                rod->length = 3.0f;

                physicsWorld.AddContactGenerator(rod.get());
                rods.push_back(std::move(rod));
            }

            {
                auto* p0 = &physicsWorld.AddParticule({ 0, 2 }, 1.0f, 0.99f);
                p0->SetInfiniteMass();
                auto* p1 = &physicsWorld.AddParticule({ 3, 2 }, 1.0f, 0.99f);

                auto rod = std::make_unique<Brise::ParticleRod>();
                rod->particle[0] = p0;
                rod->particle[1] = p1;
                rod->length = 3.0f;

                physicsWorld.AddContactGenerator(rod.get());
                rods.push_back(std::move(rod));
            }

            {
                auto* a = &physicsWorld.AddParticule({ 6, 2 }, 0.5f, 0.99f);
                a->SetInfiniteMass();                            
                auto* b = &physicsWorld.AddParticule({ 8, 4 }, 0.5f, 0.99f);
                auto* c = &physicsWorld.AddParticule({ 8, 7 }, 0.5f, 0.99f);

                auto rod1 = std::make_unique<Brise::ParticleRod>();
                rod1->particle[0] = a;
                rod1->particle[1] = b;
                rod1->length = 3.0f;

                auto rod2 = std::make_unique<Brise::ParticleRod>();
                rod2->particle[0] = b;
                rod2->particle[1] = c;
                rod2->length = 3.0f;

                physicsWorld.AddContactGenerator(rod1.get());
                physicsWorld.AddContactGenerator(rod2.get());

                rods.push_back(std::move(rod1));
                rods.push_back(std::move(rod2));
            }
        }
    };

}
