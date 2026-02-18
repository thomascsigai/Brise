#pragma once

#include<SDL3/SDL.h>
#include <vector>
#include <utils.h>
#include <iostream>

#include <demo.h>
#include <Brise/World.h>
#include <Brise/Vec2.h>
#include <Brise/PLinks.h>

namespace BriseSandbox {
    class CableBridgeDemo : public Demo {
    public:
        CableBridgeDemo() { Init(); }

    private:
        Brise::World physicsWorld;

        std::vector<Brise::Particle*> bridgeParticles;
        std::vector<std::unique_ptr<Brise::ParticleCable>> cables;

        float particleRadius = 15.f;
        const int segmentCount = 10;
        const float segmentLength = 1.2f;

    public:
        const char* GetName() const override {
            return "Bridge Demo";
        }

        void PollEvent(AppContext* appstate, SDL_Event* event) override {
            if (event->type == SDL_EVENT_KEY_DOWN) {
                if (event->key.scancode == SDL_SCANCODE_RETURN) {

                    if (cables.size() > 2) {
                        physicsWorld.RemoveContactGenerator(cables[2].get());
                        cables.erase(cables.begin() + 2);
                    }
                }
            }
        }

        void Update(float dt) override {
            physicsWorld.Update(dt);
        }

        void Render(AppContext* app) override {
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);

            const auto& particles = physicsWorld.GetParticles();

            for (auto& p : particles) {
                Utils::DrawCircle(app->renderer, p.position, particleRadius);
            }

            for (size_t i = 0; i < cables.size(); ++i) {
                Utils::DrawLine(
                    app->renderer,
                    cables[i]->particle[0]->position,
                    cables[i]->particle[1]->position
                );
            }
            Utils::DrawLine(
                app->renderer,
                bridgeParticles.front()->position,
                {-15, 1}
            );
            Utils::DrawLine(
                app->renderer,
                bridgeParticles.back()->position,
                {15, 1}
            );
            SDL_SetRenderScale(app->renderer, 1.5, 1.5);
            SDL_RenderDebugText(app->renderer, 14, 565, "Press Return to break the bridge");
            SDL_SetRenderScale(app->renderer, 1, 1);
        }

    private:
        void Init() {
            float startX = -6.f;
            float y = 1.f;

            // Create particles
            for (int i = 0; i < segmentCount; ++i) {
                float x = startX + i * segmentLength;

                Brise::Particle& p =
                    physicsWorld.AddParticule({ x, y }, 1.0f, 0.99f);

                bridgeParticles.push_back(&p);
            }

            // Fix endpoints (infinite mass)
            bridgeParticles.front()->SetInfiniteMass();
            bridgeParticles.back()->SetInfiniteMass();

            // Create cables
            for (int i = 0; i < segmentCount - 1; ++i) {
                auto cable = std::make_unique<Brise::ParticleCable>();
                cable->particle[0] = bridgeParticles[i];
                cable->particle[1] = bridgeParticles[i + 1];
                cable->maxLength = segmentLength;
                cable->restitution = 0.2f;

                physicsWorld.AddContactGenerator(cable.get());
                cables.push_back(std::move(cable));
            }
        }
    };

}