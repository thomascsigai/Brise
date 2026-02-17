#pragma once

#include <Brise/Particle.h>
#include <Brise/Vec2.h>

#include <limits>
#include <vector>

namespace Brise {

	class ParticleContact {

	public:
		// Holds the particles involved in the contact
		// Second can be null for contacts with the scenery
		Particle* particle[2];

		float restitution;
		Vec2 contactNormal;
		float penetration;

	public:

		void Resolve(float duration);
		float CalculateSeparatingVelocity() const;

	private:

		void ResolveVelocity(float duration);
		void ResolveInterpenetration(float duration);

	};

	class ParticleContactResolver {
		
	protected:

		unsigned iterations;
		unsigned iterationsUsed = 0;

	public:

		ParticleContactResolver(unsigned iterations);

		void SetIterations(unsigned iterations);
		void ResolveContacts(std::vector<ParticleContact>& contactArray, float duration);

	};

	class ParticleContactGenerator {
	public:
		virtual unsigned AddContact(ParticleContact& contact, unsigned limit) const = 0;
	};

}