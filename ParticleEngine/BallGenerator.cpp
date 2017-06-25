#include "BallGenerator.h"
#include "ParticleEngine.h"

BallGenerator::BallGenerator()
{
	renderPoints.resize(0);
	renderPoints.setPrimitiveType(sf::PrimitiveType::Lines);
	points[0] = glm::vec2();
	points[1] = glm::vec2();
	fanVec = glm::vec2();
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

	fanVec = end - start;
	spawnDirection.x = -fanVec.y;
	spawnDirection.y = fanVec.x;
	spawnDirection = glm::normalize(spawnDirection);

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
	spawnCooldown = 1.0f;
}

BallGenerator::BallGenerator(const BallGenerator& other)
	: spawnVelocity(other.spawnVelocity)
{
	points[0] = other.points[0];
	points[1] = other.points[1];

	renderPoints = other.renderPoints;
	fanVec = other.fanVec;
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

		//find random Spawn position
		glm::vec2 positon = points[0];

		Ball ball;
		ball.position = positon;
		ball.velocity = spawnDirection * spawnVelocity;

		engine.AddBall(ball);
	}
}

void BallGenerator::Render(sf::RenderWindow& window)
{
	window.draw(renderPoints);
}
