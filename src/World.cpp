#include <Brise/World.h>

namespace Brise {
	World::World(size_t numParticles) {
		Init(numParticles);
	}

	void World::Step(float deltaTime) {
		forceRegistry.UpdateForces(deltaTime);

		for (auto& p : particles) {
			p.Integrate(deltaTime);
		}
	}

	Particle& World::AddParticule(Vec2 position, float mass, float damping) {
		particles.push_back(Particle(position, mass, damping));
		particles.back().acceleration = gravity; // Set world gravity as constant acceleration

		return particles.back();
	}

	const World::ParticleContainer& World::GetParticles() const {
		return particles;
	}

	void World::Init(size_t numParticles) {
		particles.reserve(numParticles);
		SetGravity({ 0, -9.81 }); // Default to real world gravity acceleration
	}

	void World::Shutdown() {}

	void World::SetGravity(Vec2 gravityForce) {
		gravity = gravityForce;
	}

	Vec2 World::GetGravity() {
		return gravity;
	}
}