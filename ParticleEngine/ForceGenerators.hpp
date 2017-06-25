#pragma once
#include "Particle.h"

namespace ForceGenerators
{
	const static glm::vec2 g_gravity(0.0f, 9.81f);
	const static float g_airPressure = 0.995f;

	struct SpringContraint
	{
		Particle* p1;
		Particle* p2;
		float restLength;
		float stiffness;
	};

	static void ApplyGravity(Particle& particle)
	{
		particle.acceleration += g_gravity;
	}

	static void ApplyAirDrag(Particle& particle)
	{
		particle.velocity *= g_airPressure;
	}

	static void ApplySpringForces(Particle& p1, Particle& p2, float restLength, float stiffness)
	{
		glm::vec2 direction = Collisions::saveNormalize(p2.position - p1.position);
		float currentLength = Collisions::saveDistance(p1.position, p2.position);

		float force = -stiffness * (currentLength - restLength);

		//printf("Force: %f\n\r", force);
		//printf("Direction: %f, %f\n\r", direction.x, direction.y);

		p1.acceleration += force * -direction;
		p2.acceleration += -force * -direction;
	}
}