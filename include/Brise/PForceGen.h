#pragma once

#include <Brise/Particle.h>
#include <vector>

namespace Brise {

	class ParticleForceGenerator {
	public: 
		virtual void UpdateForce(Particle& particle, float duration) = 0;
	};

	class ParticleForceRegistry {
	protected:
		struct ParticleForceRegistration {
			Particle* particle;
			ParticleForceGenerator* fg;
		};

		std::vector<ParticleForceRegistration> registry;

	public:
		void Add(Particle* particle, ParticleForceGenerator* fg);
		void Remove(Particle* particle, ParticleForceGenerator* fg);
		void Clear();

		void UpdateForces(float duration);
	};

	// USEFUL GENERATORS
	
	// Gravity Generator
	class ParticleGravity : public ParticleForceGenerator {
	private:
		Vec2 gravity; // Gravity acceleration (m/s^2)

	public: 
		ParticleGravity(const Vec2& gravityForce);

		virtual void UpdateForce(Particle& particle, float duration) override;
	};

}