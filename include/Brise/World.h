#pragma once

#include <Brise/Particle.h>
#include <Brise/PForceGen.h>
#include <Brise/Vec2.h>
#include <vector>

namespace Brise {

	constexpr size_t DEFAULT_NUM_PARTICLES = 100;

	class World {

	public:
		using ParticleContainer = std::vector<Particle>;

	private:
		ParticleContainer particles;
		ParticleForceRegistry forceRegistry;
		Vec2 gravity; // World gravity acceleration

	public:

		/// <param name="numParticles">: Should be above the number of particles your world will contains.
		/// Works even if the number of particles exceeds this.</param>
		explicit World(size_t numParticles = DEFAULT_NUM_PARTICLES);

		void Step(float deltaTime);
		Particle& AddParticule(Vec2 position, float mass, float damping);
		const ParticleContainer& GetParticles() const;

	private:

		void Init(size_t numParticles);
		void Shutdown();

		void SetGravity(Vec2 gravityForce);
		Vec2 GetGravity();
	};

}