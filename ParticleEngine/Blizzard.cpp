#include "Blizzard.h"
#include "Particle.h"
#include "ParticleEngine.h"

Blizzard::Blizzard(glm::vec2 position, size_t spawnCount)
	: position(position)
{
	spawnPoints.resize(spawnCount);
	spawnDirections.resize(spawnCount);
	radius = 2.0f;

	for(size_t i = 0u; i < spawnCount; ++i)
	{
		spawnPoints[i].x = position.x + radius * cos((2.0f * Config::pi / static_cast<float>(spawnCount)) * i);
		spawnPoints[i].y = position.y + radius * sin((2.0f * Config::pi / static_cast<float>(spawnCount)) * i);

		spawnDirections[i] = spawnPoints[i] - position;
	}

	vertices.resize(spawnPoints.size());

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position.x = spawnPoints[i].x;
		vertices[i].position.y = spawnPoints[i].y;
		vertices[i].color = sf::Color::Red;
	}

	spawnTime = 0;
	spawnVelocity = 100.0f;
	spawnCooldown = 0.05f;
}

Blizzard::Blizzard(const Blizzard& other)
{
	radius = other.radius;
	vertices = other.vertices;
	spawnPoints = other.spawnPoints;
	position = other.position;
	spawnTime = other.spawnTime;
	spawnDirections = other.spawnDirections;
	spawnVelocity = other.spawnVelocity;
	spawnCooldown = other.spawnCooldown;
}

void Blizzard::DebugRender(sf::RenderWindow& window)
{
	window.draw(&vertices[0], vertices.size(), sf::PrimitiveType::Points);
}

void Blizzard::Update(float deltaTime, ParticleEngine& engine)
{
	spawnTime += deltaTime;

	Collisions::BoundingVolumes::RotateAroundPointRads(&spawnPoints[0], spawnPoints.size(), position, Config::pi * deltaTime);

	if(spawnTime > spawnCooldown)
	{
		spawnTime -= spawnCooldown;

		for(size_t i = 0u; i < spawnPoints.size(); ++i)
		{
			Particle particle;
			particle.position = spawnPoints[i];
			particle.velocity = spawnDirections[i] * spawnVelocity;

			engine.AddParticle(particle);

			spawnDirections[i] = spawnPoints[i] - position;
		}

	}


}
