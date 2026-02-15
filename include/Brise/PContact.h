#pragma once

#include <Brise/Particle.h>
#include <Brise/Vec2.h>

namespace Brise {

	class ParticleContact {

	public:
		// Holds the particles involved in the contact
		// Second can be null for contacts with the scenery
		Particle* particle[2];

		float restitution;
		Vec2 contactNormal;

	protected:
		void resolve(float duration) {
			resolveVelocity(duration);
			//ResolveInterpenetration(duration);
		}

		float calculateSeparatingVelocity() const {
			Vec2 relativeVelocity = particle[0]->velocity;
			if (particle[1]) relativeVelocity -= particle[1]->velocity;
			return Dot(relativeVelocity, contactNormal);
		}

	private:
		void resolveVelocity(float duration) {
			float separatingVelocity = calculateSeparatingVelocity();

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
				particle[1]->velocity += impulsePerIMass * -particle[1]->GetInverseMass();
			}
		}

	};

}