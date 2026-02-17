#pragma once

#include <SDL3/SDL.h>
#include <demo.h>
#include <utils.h>
#include <Brise/World.h>
#include <Brise/PContact.h>
#include <Brise/Vec2.h>

namespace BriseSandbox {

    class RestingContactDemo : public Demo
    {
    private:
        Brise::World physicsWorld;
        std::vector<Brise::Particle*> particles;

        float groundY = 0.0f;
        float radius = 0.33f;

    public:

        RestingContactDemo() { Init(); }

        const char* GetName() const override {
            return "Resting Contact Stack Demo";
        }

        void PollEvent(AppContext* appstate, SDL_Event* event) override {
        }

        void Update(double deltaTime) override
        {
            physicsWorld.Step(deltaTime);

            // Contact sol pour chaque particule
            for (auto* p : particles)
            {
                float penetration = (groundY + radius) - p->position.y;

                if (penetration > 0.0f)
                {
                    Brise::ParticleContact contact;

                    contact.particle[0] = p;
                    contact.particle[1] = nullptr;
                    contact.contactNormal = { 0, 1 };
                    contact.penetration = penetration;
                    contact.restitution = 0.0f;

                    contact.Resolve(deltaTime);
                }
            }

            // Contacts particule / particule
            for (size_t i = 0; i < particles.size(); ++i)
            {
                for (size_t j = i + 1; j < particles.size(); ++j)
                {
                    auto* p0 = particles[i];
                    auto* p1 = particles[j];

                    Brise::Vec2 delta = p0->position - p1->position;
                    float distance = Brise::Magnitude(delta);

                    float penetration = (2.0f * radius) - distance;

                    if (penetration > 0.0f)
                    {
                        Brise::ParticleContact contact;

                        contact.particle[0] = p0;
                        contact.particle[1] = p1;
                        contact.contactNormal = Normalize(delta);
                        contact.penetration = penetration;
                        contact.restitution = 0.0f;

                        contact.Resolve(deltaTime);
                    }
                }
            }
        }

        void Render(AppContext* app) override
        {
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

            for (auto* p : particles)
            {
                Utils::DrawCircle(app->renderer, p->position, 25);
                //Utils::DrawParticleInfos(app->renderer, *p);
            }

            Brise::Vec2 left = { -20, groundY };
            Brise::Vec2 right = { 20, groundY };
            Utils::DrawLine(app->renderer, left, right);
        }

    private:

        void Init()
        {
            particles.clear();

            particles.push_back(&physicsWorld.AddParticule({ 0, 3 }, 1.0f, 0.99f));
            particles.push_back(&physicsWorld.AddParticule({ 0, 4 }, 1.0f, 0.99f));
            particles.push_back(&physicsWorld.AddParticule({ 0, 5 }, 1.0f, 0.99f));

            for (auto* p : particles)
                p->velocity = { 0, 0 };
        }
    };


}
