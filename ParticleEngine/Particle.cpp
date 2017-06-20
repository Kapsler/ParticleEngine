#include "Particle.h"

Particle::Particle()
{
	position.x = 0.0f;
	position.y = 0.0f;
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

	position += velocity * deltaTime;
}

void Particle::Render(sf::RenderWindow& window, sf::CircleShape& circle)
{
	circle.setScale(0.6f, 0.6f);
	circle.setPosition(position.x, position.y);
	if(circle.getFillColor() != sf::Color::Green)
	{
		circle.setFillColor(sf::Color::Green);
	}
	if(circle.getOutlineColor() != sf::Color::Green)
	{
		circle.setOutlineColor(sf::Color::Green);
	}
	
	window.draw(circle);
}
