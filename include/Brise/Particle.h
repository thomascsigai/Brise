#pragma once

#include <Brise/Vec2.h>

namespace Brise {
	class Particle {

	public:

		Vec2 position;
		Vec2 velocity;
		Vec2 acceleration;
		
	private:

		float inverseMass;
		float damping;
		Vec2 forceAccum;

	public:

		Particle(Vec2 _pos, float mass, float damping);

		void Integrate(float dt);

		void AddForce(const Vec2& force);
		void ClearAccumulator();
		
		void SetMass(float value);
		void SetInfiniteMass();
		float GetMass() const;
		float GetInverseMass() const;
		bool HasFiniteMass();

		void SetDamping(float value);

	};
}