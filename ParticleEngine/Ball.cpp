#include "Ball.h"

Ball::Ball()
{
	radius = 1.0f;
}

bool Ball::DoesCollideWithAABB(Collisions::BoundingVolumes::AABB& aabb) const
{
	return Collisions::SphereBoxCollision(position, radius, aabb);
}

bool Ball::DoesCollideWithSphere(const glm::vec2& sphereCenter, const float sphereRadius) const
{
	return glm::distance(position, sphereCenter) <= (radius + sphereRadius);
}

void Ball::ResolveCollision(const Collisions::Contact& contact)
{
	position += (contact.penetration + 0.5f) * contact.contactNormal;

	acceleration += -((1.0f + bouncyness) * glm::dot(velocity, contact.contactNormal)) * contact.contactNormal;
}
