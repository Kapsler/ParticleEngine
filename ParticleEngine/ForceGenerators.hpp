#pragma once
#include "Particle.h"

namespace ForceGenerators
{
	static glm::vec2 g_gravity(0.0f, 9.81f);

	static void ApplyGravity(Particle& particle)
	{
		particle.acceleration += g_gravity;
	}
}