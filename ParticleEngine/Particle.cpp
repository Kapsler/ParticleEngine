#include "Particle.h"

Particle::Particle()
{
	position.x = 0.0f;
	position.y = 0.0f;
	oldPosition.x = 0.0f;
	oldPosition.y = 0.0f;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	acceleration.x = 0.0f;
	acceleration.y = 0.0f;
	mass = 1.0f;
	bouncyness = 0.8f;
	toBeDeleted = false;
}

Particle::Particle(const Particle& other)
{
	mass = other.mass;
	position = other.position;
	oldPosition = other.oldPosition;
	velocity = other.velocity;
	acceleration = other.acceleration;
	bouncyness = other.bouncyness;
	toBeDeleted = other.toBeDeleted;
}

Particle::~Particle()
{
}

void Particle::Integrate(float deltaTime)
{
	velocity += acceleration;
	oldPosition = position;
	position += velocity * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;

	acceleration.x = 0.0f;
	acceleration.y = 0.0f;
}

bool Particle::DoesCollideWithAABB(Collisions::BoundingVolumes::AABB& aabb) const
{
	if (position.x > aabb.min.x && position.x < aabb.max.x
	&&	position.y > aabb.min.y && position.y < aabb.max.y)
	{
		return true;
	}

	return false;
}

 bool inline Particle::DoesCollideWithSphere(const glm::vec2& sphereCenter, const float sphereRadius) const
{
	return glm::distance(sphereCenter, position) <= sphereRadius;
}

void Particle::ResolveCollision(const Collisions::Contact& contact)
{	
	position += (contact.penetration + 0.5f) * contact.contactNormal;

	acceleration += -((1.0f + bouncyness) * glm::dot(velocity, contact.contactNormal)) * contact.contactNormal;
}
