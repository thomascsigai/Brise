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
		unsigned iterationsUsed;

	public:

		ParticleContactResolver(unsigned iterations) 
			: iterations(iterations) {
			iterationsUsed = 0;
		}

		void SetIterations(unsigned iterations) {
			iterations = iterations;
		}

		void ResolveContacts(std::vector<ParticleContact>& contactArray, unsigned numContacts, float duration) {
			unsigned i;
			iterationsUsed = 0;

			while (iterationsUsed < iterations) {
				// Find contact with largest closing velocity
				float max = std::numeric_limits<float>::max();
				unsigned maxIndex = numContacts;
				for (i = 0; i < numContacts; i++) {
					float sepVel = contactArray[i].CalculateSeparatingVelocity();
					
					if (sepVel < max && (sepVel < 0 || contactArray[i].penetration > 0)) {
						max = sepVel;
						maxIndex = i;
					}
				}

				if (maxIndex == numContacts) break;
				contactArray[maxIndex].Resolve(duration);
				iterationsUsed++;
			}
		}

	};

	class ParticleContactGenerator {
	public:
		virtual unsigned AddContact(ParticleContact& contact, unsigned limit) const = 0;
	};

}