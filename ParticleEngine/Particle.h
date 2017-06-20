#pragma once
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"

struct Particle
{
	Particle();
	virtual ~Particle();

	virtual void Integrate(float deltaTime);
	virtual void Render(sf::RenderWindow& window, sf::CircleShape& circle);

	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	float mass;
};
