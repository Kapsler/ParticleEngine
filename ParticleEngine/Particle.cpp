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
	bouncyness = 0.2f;
	toBeDeleted = false;
	state = NO_FRICTION;
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
	state = other.state;
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

void Particle::Reflexion(const Collisions::Contact& contact)
{	
	float speed = Collisions::saveLength(velocity);

	if (speed < 1e-3f)
	{
		state = STATIC_FRICTION;
	}
	//NormalKraft abfragen
	else if (false)
	{
		state = KINEMATIC_FRICTION;
	}
	else
	{
		state = NO_FRICTION;
	}


}
