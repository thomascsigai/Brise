#include <Brise/PContact.h>

namespace Brise {
	void ParticleContact::Resolve(float duration) {
		ResolveVelocity(duration);
		ResolveInterpenetration(duration);
	}

	float ParticleContact::CalculateSeparatingVelocity() const {
		Vec2 relativeVelocity = particle[0]->velocity;
		if (particle[1]) relativeVelocity -= particle[1]->velocity;
		return Dot(relativeVelocity, contactNormal);
	}

	void ParticleContact::ResolveVelocity(float duration) {
		float separatingVelocity = CalculateSeparatingVelocity();

		// Checks if particles are separating
		if (separatingVelocity > 0) return; // no impulse required

		// Calculate new separating velocity
		float newSepVelocity = -separatingVelocity * restitution;

		// Checks the velocity buildup due to acceleration only
		Vec2 accCausedVelocity = particle[0]->acceleration;
		if (particle[1]) accCausedVelocity -= particle[1]->acceleration;
		float accCausedSepVelocity = Dot(accCausedVelocity, contactNormal) * duration;

		// If ther's closing vel due to acceleration buildup, remove it
		if (accCausedSepVelocity < 0) {
			newSepVelocity += restitution * accCausedSepVelocity;

			if (newSepVelocity < 0) newSepVelocity = 0;
		}

		float deltaVelocity = newSepVelocity - separatingVelocity;

		// Apply the change in proportion of the inverseMass
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
			particle[1]->velocity += impulsePerIMass * (-particle[1]->GetInverseMass());
		}
	}

	void ParticleContact::ResolveInterpenetration(float duration) {
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
			particleMovement1 = movePerIMass * (-particle[1]->GetInverseMass());
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

	ParticleContactResolver::ParticleContactResolver(unsigned iterations)
		: iterations(iterations) {
	}

	void ParticleContactResolver::SetIterations(unsigned iterations) {
		iterations = iterations;
	}

	void ParticleContactResolver::ResolveContacts(std::vector<ParticleContact>& contactArray, unsigned numContacts, float duration) {
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
}