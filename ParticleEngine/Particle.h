#pragma once
#include "glm/glm.hpp"
#include "Collision.hpp"

struct Particle
{
	Particle();
	Particle(const Particle& other);
	virtual ~Particle();

	void Integrate(float deltaTime);

	void ResolveCollision(const Collisions::Contact& contact);

	glm::vec2 oldPosition;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	float mass;
	float bouncyness;
	bool toBeDeleted : 1;
};
