#include <Brise/Particle.h>
#include <Brise/BriseAssert.h>
#include <limits>

namespace Brise {
	Particle::Particle(Vec2 position, float mass, float damping) 
	: position(position), damping(damping) {
		SetMass(mass);
	}

	/// <summary>
	/// Integrates the particle forward in time by the given amount.
	/// Using Newton Euler integration.
	/// </summary>
	/// <param name="duration">In ms</param>
	void Particle::Integrate(float duration) {
		// Don't integrate particles with infinite mass
		if (inverseMass <= 0) return;

		BR_ASSERT(duration > 0);

		// Update Linear Position
		position += velocity * duration;

		// Update velocity from acceleration
		velocity += acceleration * duration;

		// Impose Drag
		//velocity *= pow(damping, duration);
	}

	void Particle::SetMass(float mass) {
		BR_ASSERT(mass > 0);

		inverseMass = 1 / mass;
	}

	void Particle::SetInfiniteMass() {
		inverseMass = 0;
	}

	float Particle::GetMass() {
		if (inverseMass == 0) return std::numeric_limits<float>::max();
		
		return (1.0f / inverseMass);
	}
}