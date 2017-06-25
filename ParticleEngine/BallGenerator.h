#pragma once
#include <glm/glm.hpp>
#include "sfml/Graphics.hpp"

class ParticleEngine;

class BallGenerator
{
public:
	BallGenerator();
	BallGenerator(const glm::vec2& start, const glm::vec2& end, float spawnVelocity);
	BallGenerator(const BallGenerator& other);
	~BallGenerator();

	void Update(float deltaTime, ParticleEngine& engine);
	void Render(sf::RenderWindow& window);

private:
	glm::vec2 GetRandomSpawnPoint();

	//Start and Endpoint
	glm::vec2 points[2];
	glm::vec2 spawnVector;
	glm::vec2 spawnDirection;
	float spawnVelocity;
	float spawnTime;
	float spawnCooldown;

	//Renderstuff
	sf::VertexArray renderPoints;
};
