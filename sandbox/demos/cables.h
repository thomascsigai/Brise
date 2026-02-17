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
        std::vector<std::unique_ptr<Brise::ParticleCable>> cables;

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

            for (const auto& cable : cables)
            {
                Utils::DrawLine(
                    app->renderer,
                    cable->particle[0]->position,
                    cable->particle[1]->position
                );
            }
        }

    private:

        void Init()
        {
            {
                auto* p0 = &physicsWorld.AddParticule({ -5, 3 }, 1.0f, 0.99f);
                p0->SetInfiniteMass();
                auto* p1 = &physicsWorld.AddParticule({ -2, 0 }, 1.0f, 0.99f);

                auto cable = std::make_unique<Brise::ParticleCable>();
                cable->particle[0] = p0;
                cable->particle[1] = p1;
                cable->maxLength = 3.0f;
                cable->restitution = 0.3f;

                physicsWorld.AddContactGenerator(cable.get());
                cables.push_back(std::move(cable));
            }

            {
                auto* p0 = &physicsWorld.AddParticule({ -5, -1}, 1.0f, 0.99f);
                auto* p1 = &physicsWorld.AddParticule({ -2, -3 }, 1.0f, 0.99f);
                p0->acceleration = { 0, 0 };
                p0->velocity = { 3, 0 };
                p1->acceleration = { 0, 0 };

                auto cable = std::make_unique<Brise::ParticleCable>();
                cable->particle[0] = p0;
                cable->particle[1] = p1;
                cable->maxLength = 3.0f;
                cable->restitution = 0.3f;

                physicsWorld.AddContactGenerator(cable.get());
                cables.push_back(std::move(cable));
            }

            {
                auto* a = &physicsWorld.AddParticule({ 5, 3 }, 1.0f, 0.99f);
                a->SetInfiniteMass();
                auto* b = &physicsWorld.AddParticule({ 7, 1 }, 1.0f, 0.99f);
                auto* c = &physicsWorld.AddParticule({ 9, 3 }, 1.0f, 0.99f);

                auto cable1 = std::make_unique<Brise::ParticleCable>();
                cable1->particle[0] = a;
                cable1->particle[1] = b;
                cable1->maxLength = 3.0f;
                cable1->restitution = 0.3f;

                auto cable2 = std::make_unique<Brise::ParticleCable>();
                cable2->particle[0] = b;
                cable2->particle[1] = c;
                cable2->maxLength = 3.0f;
                cable2->restitution = 0.3f;

                physicsWorld.AddContactGenerator(cable1.get());
                physicsWorld.AddContactGenerator(cable2.get());

                cables.push_back(std::move(cable1));
                cables.push_back(std::move(cable2));
            }
        }
    };

}
