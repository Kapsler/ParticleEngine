#include "Ball.h"

bool Ball::DoesCollideWithAABB(Collisions::BoundingVolumes::BoundingBox& aabb) const
{
	return false;
}

bool Ball::DoesCollideWithSphere(const glm::vec2& sphereCenter, const float sphereRadius) const
{
	return glm::distance(position, sphereCenter) < (radius + sphereRadius);
}