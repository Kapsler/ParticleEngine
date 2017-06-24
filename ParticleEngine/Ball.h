#pragma once
#include "Particle.h"

struct Ball : public Particle
{
	Ball();

	bool DoesCollideWithAABB(Collisions::BoundingVolumes::AABB& aabb) const;
	bool inline DoesCollideWithSphere(const glm::vec2& sphereCenter, const float sphereRadius) const;
	void ResolveCollision(const Collisions::Contact& contact);

	float radius;
};
