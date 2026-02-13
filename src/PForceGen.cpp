#include <Brise/PForceGen.h>
#include <Brise/BriseAssert.h>

namespace Brise {
	void ParticleForceRegistry::Add(Particle* particle, ParticleForceGenerator* fg) {
		for (const auto& reg : registry) {
			if (reg.particle == particle && reg.fg == fg)
				return; // ignore duplicate
		}
		registry.push_back({ particle, fg });
	}

	void ParticleForceRegistry::Remove(Particle* particle, ParticleForceGenerator* fg) {
		registry.erase(
			std::remove_if(
				registry.begin(),
				registry.end(),
				[&](const ParticleForceRegistration& reg)
				{
					return reg.particle == particle &&
						reg.fg == fg;
				}),
			registry.end()
		);
	}

	void ParticleForceRegistry::Clear() {
		registry.clear();
	}

	void ParticleForceRegistry::UpdateForces(float duration) {
		for (auto& reg : registry) {
			reg.fg->UpdateForce(reg.particle, duration);
		}
	}

	// GRAVITY

	ParticleGravity::ParticleGravity(const Vec2& gravityForce) 
	: gravity(gravityForce) {}

	void ParticleGravity::UpdateForce(Particle* particle, float duration) {
		if (not particle->HasFiniteMass()) return;

		particle->AddForce(gravity * particle->GetMass());
	}

	// SPRINGS

	ParticleSpring::ParticleSpring(Particle* other, float springConstant, float restLength)
		: other(other), springConstant(springConstant), restLength(restLength) {}

	void ParticleSpring::UpdateForce(Particle* particle, float duration) {
		Vec2 force = particle->position - other->position;

		float length = Magnitude(force);
		if (length <= 0.0001f) return;

		float magnitude = (length - restLength);
		magnitude *= springConstant;

		force = Normalize(force);

		// Hooke : F = -k (x - L0)
		force *= -magnitude;

		particle->AddForce(force);
	}

	// ANCHORED SPRING

	AnchoredParticleSpring::AnchoredParticleSpring(Vec2 anchor, float springConstant, float restLength)
		: anchor(anchor), springConstant(springConstant), restLength(restLength) {}

	void AnchoredParticleSpring::UpdateForce(Particle* particle, float duration) {
		Vec2 delta = particle->position - anchor;

		float length = Magnitude(delta);
		if (length <= 0.0001f) return;

		float displacement = length - restLength;

		Vec2 force = delta / length;
		force *= -springConstant * displacement;

		particle->AddForce(force);
	}

	// BUNGEE SPRING

	ParticleBungee::ParticleBungee(Particle* other, float springConstant, float restLength)
		: other(other), springConstant(springConstant), restLength(restLength) { }

	void ParticleBungee::UpdateForce(Particle* particle, float duration) {
		Vec2 delta = particle->position - other->position;

		float length = Magnitude(delta);
		if (length <= 0.0001f) return;

		if (length <= restLength)
			return;

		float displacement = length - restLength;

		Vec2 force = delta / length;
		force *= -springConstant * displacement;

		particle->AddForce(force);
	}
}