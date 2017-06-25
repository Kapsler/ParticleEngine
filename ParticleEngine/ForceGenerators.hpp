#pragma once
#include "Particle.h"

namespace ForceGenerators
{
	static glm::vec2 g_gravity(0.0f, 9.81f);
	static float airPressure = 0.995f;

	static void ApplyGravity(Particle& particle)
	{
		particle.acceleration += g_gravity;
	}

	static void ApplyAirDrag(Particle& particle)
	{
		particle.velocity *= airPressure;
	}
}