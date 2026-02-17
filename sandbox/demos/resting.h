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

        std::vector<Brise::Particle*> single;
        std::vector<Brise::Particle*> stack;

        float groundY = 0.0f;
        float radius = 0.33f;

    public:

        RestingContactDemo() { Init(); }

        const char* GetName() const override {
            return "Resting Contact Comparison";
        }

        void PollEvent(AppContext* appstate, SDL_Event* event) override {}

        void Update(double deltaTime) override
        {
            physicsWorld.Step(deltaTime);

            HandleGroundContacts(single, deltaTime);
            HandleGroundContacts(stack, deltaTime);

            HandleParticleContacts(stack, deltaTime);
        }

        void Render(AppContext* app) override
        {
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

            for (auto* p : single)
            {
                Utils::DrawCircle(app->renderer, p->position, 25);
                Utils::DrawParticleInfos(app->renderer, *p);
            }

            for (auto* p : stack)
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
            single.clear();
            stack.clear();

            // -------- Left side : single particle --------
            single.push_back(
                &physicsWorld.AddParticule({ -4, 5 }, 1.0f, 0.99f)
            );

            single[0]->velocity = { 0, 0 };

            // -------- Right side : stack --------
            stack.push_back(
                &physicsWorld.AddParticule({ 4, 3 }, 1.0f, 0.99f)
            );
            stack.push_back(
                &physicsWorld.AddParticule({ 4, 4 }, 1.0f, 0.99f)
            );
            stack.push_back(
                &physicsWorld.AddParticule({ 4, 5 }, 1.0f, 0.99f)
            );

            for (auto* p : stack)
                p->velocity = { 0, 0 };
        }

        void HandleGroundContacts(std::vector<Brise::Particle*>& particles, float dt)
        {
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

                    contact.Resolve(dt);
                }
            }
        }

        void HandleParticleContacts(std::vector<Brise::Particle*>& particles, float dt)
        {
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

                        contact.Resolve(dt);
                    }
                }
            }
        }
    };



}
