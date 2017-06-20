#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "sfml/Graphics.hpp"

class ParticleEngine;

class Blizzard
{
public:
	Blizzard(glm::vec2 position, size_t spawnCount);
	Blizzard(const Blizzard& other);

	void DebugRender(sf::RenderWindow& window);
	void Update(float deltaTime, ParticleEngine& engine);

private:

	std::vector<glm::vec2> spawnPoints;
	std::vector<glm::vec2> spawnDirections;
	glm::vec2 position;
	float radius;
	float spawnVelocity;
	float spawnCooldown;
	size_t spawnIndex;

	//Renderstuff
	std::vector<sf::Vertex> vertices;
};
