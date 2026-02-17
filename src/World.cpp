#include <Brise/World.h>

namespace Brise {
	World::World(size_t numParticles) 
	: resolver(0) {
		Init(numParticles);
	}

	void World::Step(float deltaTime) {
		// Apply the force generators
		forceRegistry.UpdateForces(deltaTime);

		// Integrate the particles
		for (auto& p : particles) {
			p.Integrate(deltaTime);
		}

		// Generate Contacts
		unsigned usedContacts = GenerateContacts();

		// Process the contacts
		if (usedContacts) {
			resolver.SetIterations(usedContacts * 2);
			resolver.ResolveContacts(contacts, usedContacts, deltaTime);
		}
	}

	Particle& World::AddParticule(Vec2 position, float mass, float damping) {
		particles.push_back(Particle(position, mass, damping));
		particles.back().acceleration = gravity; // Set world gravity as constant acceleration

		return particles.back();
	}

	void World::AddForceGenToRegistry(Particle* particle, ParticleForceGenerator* fg) {
		forceRegistry.Add(particle, fg);
	}

	const World::ParticleContainer& World::GetParticles() const {
		return particles;
	}

	void World::Init(size_t numParticles) {
		particles.reserve(numParticles);
		SetGravity({ 0, -9.81 }); // Default to real world gravity acceleration
		maxContacts = 100;
		contacts.resize(maxContacts);
	}

	void World::Shutdown() {}

	void World::SetGravity(Vec2 gravityForce) {
		gravity = gravityForce;
	}

	Vec2 World::GetGravity() {
		return gravity;
	}

	unsigned World::GenerateContacts() {
		unsigned limit = maxContacts;
		unsigned nextContact = 0;

		for (auto generator : contactGenerators) {
			if (nextContact >= limit)
				break;

			unsigned used = generator->AddContact(
				contacts[nextContact],
				limit - nextContact
			);

			nextContact += used;
		}

		return nextContact;
	}

	void World::AddContactGenerator(ParticleContactGenerator* generator) {
		contactGenerators.push_back(generator);
	}
}