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
	mass = 0.0f;
}

Particle::Particle(const Particle& other)
{
	mass = other.mass;
	position = other.position;
	oldPosition = other.oldPosition;
	velocity = other.velocity;
	acceleration = other.acceleration;
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

bool Particle::DoesCollideWithAABB(Collisions::BoundingVolumes::BoundingBox& aabb) const
{
	if (position.x > aabb.center.x - aabb.halfSize.x && position.x < aabb.center.x + aabb.halfSize.x
	&&	position.y > aabb.center.y - aabb.halfSize.y && position.y < aabb.center.y + aabb.halfSize.y)
	{
		return true;
	}

	return false;
}

void Particle::ResolveCollision(const Collisions::Contact& contact)
{	
	position += (contact.penetration + 0.5f) * contact.contactNormal;

	acceleration += -(2.0f * glm::dot(velocity, contact.contactNormal)) * contact.contactNormal;
}