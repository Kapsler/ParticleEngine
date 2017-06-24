#pragma once
#include "Particle.h"

struct Ball : public Particle
{
	Ball();

	//MEMBER HIDING INTENDED - Polymophism not used!
	bool DoesCollideWithAABB(Collisions::BoundingVolumes::BoundingBox& aabb) const;
	bool inline DoesCollideWithSphere(const glm::vec2& sphereCenter, const float sphereRadius) const;

	float radius;
};
