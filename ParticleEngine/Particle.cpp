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

Particle::~Particle()
{
}

void Particle::Integrate(float deltaTime)
{
	velocity += acceleration;
	acceleration.x = 0.0f;
	acceleration.y = 0.0f;

	oldPosition = position;
	position += velocity * deltaTime;
}

bool Particle::DoesCollideWithAABB(Collisions::BoundingVolumes::BoundingBox& aabb) const
{
	if(	position.x > aabb.points[0].x && position.x < aabb.points[2].x
	&&	position.y > aabb.points[0].y && position.y < aabb.points[2].y)
	{
		return true;
	}

	return false;
}

void Particle::ResolveCollisionWithAABB(Collisions::BoundingVolumes::BoundingBox& aabb)
{
	//Needs correct line!
	glm::vec2 surface = aabb.points[1] - aabb.points[0];

	glm::vec2 surfaceNormal;
	surfaceNormal.x = -surface.y;
	surfaceNormal.y = surface.x;
	surfaceNormal = glm::normalize(surfaceNormal);
	
	acceleration += -(2.0f * glm::dot(velocity, surfaceNormal)) * surfaceNormal;
}
