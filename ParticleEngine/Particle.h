#pragma once
#include "glm/glm.hpp"

struct Particle
{
	Particle();
	virtual ~Particle();

	virtual void Integrate(float deltaTime);

	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	float mass;
};
