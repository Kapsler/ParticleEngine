#include "Ball.h"

Ball::Ball()
{
	radius = 10.0f;
	mass = 10.0f;
	bouncyness = 0.8f;
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
	position += (contact.penetration) * contact.contactNormal;

	acceleration += -((1.0f + bouncyness) * glm::dot(velocity, contact.contactNormal)) * contact.contactNormal;
}
