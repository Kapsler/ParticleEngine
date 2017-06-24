#pragma once
#include "glm/glm.hpp"
#include "Collision.hpp"

struct Particle
{
	Particle();
	Particle(const Particle& other);
	virtual ~Particle();

	void Integrate(float deltaTime);

	//INTENTIONALLY HIDDEN IN DERIVED BALL
	bool DoesCollideWithAABB(Collisions::BoundingVolumes::BoundingBox& aabb) const;
	inline bool DoesCollideWithSphere(const glm::vec2& sphereCenter, const float sphereRadius) const;

	void ResolveCollision(const Collisions::Contact& contact);

	glm::vec2 oldPosition;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	float mass;
	float bouncyness;
};
