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

	public:

		Particle(Vec2 _pos, float mass, float damping);

		void Integrate(float dt);
		
		void SetMass(float mass);
		void SetInfiniteMass();
		float GetMass();

	};
}