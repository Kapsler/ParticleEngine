#pragma once
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "Particle.h"


class Fan
{
public:
	Fan();
	Fan(const glm::vec2& start, const glm::vec2& end, float strength);
	Fan(const Fan& other);
	~Fan();

	void Render(sf::RenderWindow& window);
	void InfluenceParticle(Particle& particle);

private:
	//Start and Endpoint
	glm::vec2 points[2];
	glm::vec2 fanVec;
	glm::vec2 blowDirection;
	float strength;

	//Renderstuff
	sf::VertexArray renderPoints;
};
