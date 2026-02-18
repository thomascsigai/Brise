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
    class GroundContactGenerator : public Brise::ParticleContactGenerator {
    public:
        std::vector<Brise::Particle>* particles;
        float groundY;

        GroundContactGenerator(std::vector<Brise::Particle>* particles, float groundY)
            : particles(particles), groundY(groundY) {
        }

        virtual unsigned AddContact(Brise::ParticleContact& contact, unsigned limit) const override {
            unsigned used = 0;

            for (auto& p : *particles) {
                if (p.position.y < groundY) {
                    contact.particle[0] = const_cast<Brise::Particle*>(&p);
                    contact.particle[1] = nullptr;
                    contact.contactNormal = { 0, 1 };
                    contact.penetration = groundY - p.position.y;
                    contact.restitution = 0.2f;
                    return 1;
                }
            }

            return 0;
        }
    };


    class CubeRodDemo : public Demo {
    public:
        CubeRodDemo() { Init(); }
        ~CubeRodDemo() {}

    private:
        Brise::World physicsWorld;

        Brise::Particle* p0;
        Brise::Particle* p1;
        Brise::Particle* p2;
        Brise::Particle* p3;

        std::vector<std::unique_ptr<Brise::ParticleRod>> rods;
        std::unique_ptr<GroundContactGenerator> ground;

        float size = 1.0f;
        float particleRadius = 20.0f;

    public:
        const char* GetName() const override {
            return "Cube Demo";
        }

        void PollEvent(AppContext* appstate, SDL_Event* event) override {}

        void Update(float dt) override {
            physicsWorld.Update(dt);
        }

        void Render(AppContext* app) override {
            const auto& particles = physicsWorld.GetParticles();

            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);

            for (auto& p : particles) {
                Utils::DrawCircle(app->renderer, p.position, particleRadius);
            }

            // Draw rods
            Utils::DrawLine(app->renderer, p0->position, p1->position);
            Utils::DrawLine(app->renderer, p1->position, p2->position);
            Utils::DrawLine(app->renderer, p2->position, p3->position);
            Utils::DrawLine(app->renderer, p3->position, p0->position);

            Utils::DrawLine(app->renderer, p0->position, p2->position);
            Utils::DrawLine(app->renderer, p1->position, p3->position);

            // Draw ground
            Utils::DrawLine(app->renderer, { -10,0 }, { 10,0 });
        }

    private:
        void Init() {
            physicsWorld = Brise::World(100, 1.0f / 60.0f);

            float startY = 3.0f;

            p0 = &physicsWorld.AddParticule({ -size, startY }, 1.0f, 0.99f);
            p1 = &physicsWorld.AddParticule({ size, startY }, 1.0f, 0.99f);
            p2 = &physicsWorld.AddParticule({ size, startY - 2 * size }, 1.0f, 0.99f);
            p3 = &physicsWorld.AddParticule({ -size, startY - 2 * size }, 1.0f, 0.99f);

            float edge = 2 * size;
            float diagonal = std::sqrt(2) * edge;

            auto makeRod = [&](Brise::Particle* a, Brise::Particle* b, float length) {
                auto rod = std::make_unique<Brise::ParticleRod>();
                rod->particle[0] = a;
                rod->particle[1] = b;
                rod->length = length;
                physicsWorld.AddContactGenerator(rod.get());
                rods.push_back(std::move(rod));
                };

            makeRod(p0, p1, edge);
            makeRod(p1, p2, edge);
            makeRod(p2, p3, edge);
            makeRod(p3, p0, edge);

            makeRod(p0, p2, diagonal);
            makeRod(p1, p3, diagonal);

            ground = std::make_unique<GroundContactGenerator>(
                const_cast<std::vector<Brise::Particle>*>(&physicsWorld.GetParticles()),
                0.0f
            );

            physicsWorld.AddContactGenerator(ground.get());
        }
    };
}