#pragma once

#include <Brise/Particle.h>
#include <Brise/Vec2.h>

#include <limits>

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

		void Resolve(float duration) {
			ResolveVelocity(duration);
			ResolveInterpenetration(duration);
		}

		float CalculateSeparatingVelocity() const {
			Vec2 relativeVelocity = particle[0]->velocity;
			if (particle[1]) relativeVelocity -= particle[1]->velocity;
			return Dot(relativeVelocity, contactNormal);
		}

	private:
		void ResolveVelocity(float duration) {
			float separatingVelocity = CalculateSeparatingVelocity();

			// checks if particles are separating
			if (separatingVelocity > 0) return; // no impulse required

			// calculate new separating velocity
			float newSepVelocity = -separatingVelocity * restitution;
			float deltaVelocity = newSepVelocity - separatingVelocity;

			// apply the change in proportion of the inverseMass
			float totalInverseMass = particle[0]->GetInverseMass();
			if (particle[1]) totalInverseMass += particle[1]->GetInverseMass();

			// Checks if both particles have infiniteMass
			if (totalInverseMass <= 0) return;

			// Calculate the impulse
			float impulse = deltaVelocity / totalInverseMass;
			Vec2 impulsePerIMass = contactNormal * impulse;

			// Apply impulse
			particle[0]->velocity += impulsePerIMass * particle[0]->GetInverseMass();
			if (particle[1]) {
				particle[1]->velocity += impulsePerIMass * (- particle[1]->GetInverseMass());
			}
		}

		void ResolveInterpenetration(float duration) {
			// Checks if no penetration
			if (penetration <= 0) return;

			// apply the change in proportion of the inverseMass
			float totalInverseMass = particle[0]->GetInverseMass();
			if (particle[1]) totalInverseMass += particle[1]->GetInverseMass();

			// Checks if both particles have infiniteMass
			if (totalInverseMass <= 0) return;

			// Calculate the movement amounts
			Vec2 movePerIMass = contactNormal * (penetration / totalInverseMass);
			
			Vec2 particleMovement0, particleMovement1;
			particleMovement0 = movePerIMass * particle[0]->GetInverseMass();
			if (particle[1]) {
				particleMovement1 = movePerIMass * (- particle[1]->GetInverseMass());
			}
			else {
				particleMovement1 = { 0, 0 };
			}

			// Apply penetration resolution
			particle[0]->position += particleMovement0;
			if (particle[1]) {
				particle[1]->position += particleMovement1;
			}
		}

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