#pragma once

#include <Brise/Particle.h>
#include <Brise/PForceGen.h>
#include <Brise/PContact.h>
#include <Brise/Vec2.h>
#include <vector>

namespace Brise {

	constexpr size_t DEFAULT_NUM_PARTICLES = 100;

	class World {

	public:
		using ParticleContainer = std::vector<Particle>;
		using ContactGenerators = std::vector<ParticleContactGenerator*>;
		using ParticleContacts = std::vector<ParticleContact>;

		ParticleContacts contacts;
		ParticleContactResolver resolver;
		ContactGenerators contactGenerators;
	
	private:
		ParticleContainer particles;
		ParticleForceRegistry forceRegistry;
		
		unsigned maxContacts;

		Vec2 gravity; // World gravity acceleration

		float fixedDt;
		float accumulator = 0;

	public:

		// numParticles should be above the number of particles your world will contains
		explicit World(size_t numParticles = DEFAULT_NUM_PARTICLES, float fixedTimeStep = 1.0f / 120.0f);

		void Update(float deltaTime);

		Particle& AddParticule(Vec2 position, float mass, float damping);
		const ParticleContainer& GetParticles() const;

		void AddForceGenToRegistry(Particle* particle, ParticleForceGenerator* fg);
		
		void AddContactGenerator(ParticleContactGenerator* generator);
		void RemoveContactGenerator(ParticleContactGenerator* generator);

	private:
		void Init(size_t numParticles);
		void Shutdown();

		void Step(float fixedDt);

		void SetGravity(Vec2 gravityForce);
		Vec2 GetGravity();

		unsigned GenerateContacts();
	};

}