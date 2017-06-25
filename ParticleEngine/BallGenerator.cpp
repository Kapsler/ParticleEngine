#include "BallGenerator.h"
#include "ParticleEngine.h"
#include "StaticXORShift.hpp"

BallGenerator::BallGenerator()
{
	renderPoints.resize(0);
	renderPoints.setPrimitiveType(sf::PrimitiveType::Lines);
	points[0] = glm::vec2();
	points[1] = glm::vec2();
	spawnVector = glm::vec2();
	spawnVelocity = 0.0f;
	spawnDirection = glm::vec2();
	spawnTime = 0.0f;
	spawnCooldown = 0.00f;
}

BallGenerator::BallGenerator(const glm::vec2& start, const glm::vec2& end, float spawnVelocity)
	: spawnVelocity(spawnVelocity)
{
	points[0] = start;
	points[1] = end;

	spawnVector = end - start;
	spawnDirection.x = -spawnVector.y;
	spawnDirection.y = spawnVector.x;
	spawnDirection = glm::normalize(spawnDirection);
	spawnVector = glm::normalize(spawnVector);

	renderPoints.setPrimitiveType(sf::PrimitiveType::Lines);
	renderPoints.resize(4);

	renderPoints[0].position.x = start.x;
	renderPoints[0].position.y = start.y;
	renderPoints[0].color = sf::Color::Red;
	renderPoints[1].position.x = end.x;
	renderPoints[1].position.y = end.y;
	renderPoints[1].color = sf::Color::Red;
	renderPoints[2].position.x = (start + end).x * 0.5f;
	renderPoints[2].position.y = (start + end).y * 0.5f;
	renderPoints[2].color = sf::Color::Red;
	renderPoints[3].position.x = renderPoints[2].position.x + (spawnDirection.x * spawnVelocity);
	renderPoints[3].position.y = renderPoints[2].position.y + (spawnDirection.y * spawnVelocity);
	renderPoints[3].color = sf::Color::Red;

	spawnTime = 0.0f;
	spawnCooldown = 0.5f;
}

BallGenerator::BallGenerator(const BallGenerator& other)
	: spawnVelocity(other.spawnVelocity)
{
	points[0] = other.points[0];
	points[1] = other.points[1];

	renderPoints = other.renderPoints;
	spawnVector = other.spawnVector;
	spawnDirection = other.spawnDirection;
	spawnTime = other.spawnTime;
	spawnCooldown = other.spawnCooldown;
}

BallGenerator::~BallGenerator()
{
}

void BallGenerator::Update(float deltaTime, ParticleEngine& engine)
{
	spawnTime += deltaTime;

	if (spawnTime > spawnCooldown)
	{
		spawnTime -= spawnCooldown;

		Ball ball;
		ball.position = GetRandomSpawnPoint();
		ball.velocity = spawnDirection * spawnVelocity;

		engine.AddBall(ball);
	}
}

void BallGenerator::Render(sf::RenderWindow& window)
{
	window.draw(renderPoints);
}

glm::vec2 BallGenerator::GetRandomSpawnPoint()
{
	return points[0] + (glm::distance(points[0], points[1]) * StaticXorShift::GetZeroToOne()) * spawnVector;
}
