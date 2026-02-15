#include <Brise/Particle.h>
#include <Brise/BriseAssert.h>
#include <limits>

namespace Brise {
	Particle::Particle(Vec2 position, float mass, float damping) 
	: position(position), damping(damping) {
		SetMass(mass);
		velocity = { 0, 0 };
		acceleration = { 0, 0 };

		forceAccum = { 0, 0 };
	}

	/// <summary>
	/// Integrates the particle forward in time by the given amount.
	/// Using Newton Euler integration.
	/// </summary>
	/// <param name="duration">In ms</param>
	void Particle::Integrate(float duration) {
		// Don't integrate particles with infinite mass
		if (inverseMass == 0) return;

		BR_ASSERT(duration > 0);

		// Update Linear Position
		position += velocity * duration;

		// Get acceleration from the forces
		Vec2 resultingAcc = acceleration;
		resultingAcc += forceAccum * inverseMass;

		// Update velocity from acceleration
		velocity += resultingAcc * duration;

		// Impose Drag
		velocity *= pow(damping, duration);

		// Clear the accumulator
		ClearAccumulator();
	}

	void Particle::SetMass(float value) {
		BR_ASSERT(value > 0);

		inverseMass = 1 / value;
	}

	void Particle::SetInfiniteMass() {
		inverseMass = 0;
	}

	float Particle::GetMass() const {
		if (inverseMass == 0) return std::numeric_limits<float>::max();
		
		return (1.0f / inverseMass);
	}

	float Particle::GetInverseMass() const {
		return inverseMass;
	}

	void Particle::AddForce(const Vec2& force) {
		forceAccum += force;
	}

	void Particle::ClearAccumulator() {
		forceAccum = { 0, 0 };
	}

	bool Particle::HasFiniteMass() {
		return (inverseMass > 0 ? true : false);
	}

	void Particle::SetDamping(float value) {
		damping = value;
	}
}