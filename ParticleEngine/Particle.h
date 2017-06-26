#pragma once
#include "glm/glm.hpp"
#include "Collision.hpp"

struct Particle
{
	enum FrictionState
	{
		STATIC_FRICTION,
		KINEMATIC_FRICTION,
		NO_FRICTION
	};

	Particle();
	Particle(const Particle& other);
	virtual ~Particle();

	void Integrate(float deltaTime);

	void Reflexion(const Collisions::Contact& contact);

	glm::vec2 oldPosition;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	float mass;
	float bouncyness;
	bool toBeDeleted : 1;
	FrictionState state;
};
