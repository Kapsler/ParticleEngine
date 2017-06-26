#include "Ball.h"

Ball::Ball()
{
	radius = Config::ballSize;
	mass = 10.0f;
	bounciness = 0.7f;
	inverseMass = 1.0f / mass;
	staticFriction = 0.25f;
	kinematicFriction = 0.1f;
}
