#pragma once
#include "glm/glm.hpp"

struct Particle
{
	Particle();
	Particle(const Particle& other);
	virtual ~Particle();

	void Integrate(float deltaTime);

	glm::vec2 oldPosition;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	float mass;
	float inverseMass;
	float bounciness;
	bool toBeDeleted : 1;
	float staticFriction;
	float kinematicFriction;
};
