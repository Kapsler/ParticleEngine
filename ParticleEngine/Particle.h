#pragma once
#include "glm/glm.hpp"
#include "Collision.hpp"

struct Particle
{
	Particle();
	virtual ~Particle();

	void Integrate(float deltaTime);
	bool DoesCollideWithAABB(Collisions::BoundingVolumes::BoundingBox& aabb) const;
	void ResolveCollisionWithAABB(Collisions::BoundingVolumes::BoundingBox& aabb);

	glm::vec2 oldPosition;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	float mass;
};
