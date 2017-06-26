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
	bounciness = 0.1f;
	toBeDeleted = false;
	inverseMass = 1.0f / mass;
	staticFriction = 0.9f;
	kinematicFriction = 0.7f;
}

Particle::Particle(const Particle& other)
{
	mass = other.mass;
	position = other.position;
	oldPosition = other.oldPosition;
	velocity = other.velocity;
	acceleration = other.acceleration;
	bounciness = other.bounciness;
	toBeDeleted = other.toBeDeleted;
	inverseMass = other.inverseMass;
	staticFriction = other.staticFriction;
	kinematicFriction = other.kinematicFriction;
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
