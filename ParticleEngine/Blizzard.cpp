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
		spawnPoints[i].x = position.x + radius * cos((2.0f * 3.1415926f / static_cast<float>(spawnCount)) * i);
		spawnPoints[i].y = position.y + radius * sin((2.0f * 3.1415926f / static_cast<float>(spawnCount)) * i);

		spawnDirections[i] = spawnPoints[i] - position;
	}

	vertices.resize(spawnPoints.size());

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position.x = spawnPoints[i].x;
		vertices[i].position.y = spawnPoints[i].y;
		vertices[i].color = sf::Color::Red;
	}

	spawnIndex = 0;
	spawnCooldown = 0;
	spawnVelocity = 100.0f;
}

Blizzard::Blizzard(const Blizzard& other)
{
	radius = other.radius;
	vertices = other.vertices;
	spawnPoints = other.spawnPoints;
	position = other.position;
	spawnCooldown = other.spawnCooldown;
	spawnIndex = other.spawnIndex;
	spawnDirections = other.spawnDirections;
	spawnVelocity = other.spawnVelocity;
}

void Blizzard::DebugRender(sf::RenderWindow& window)
{
	window.draw(&vertices[0], vertices.size(), sf::PrimitiveType::Points);
}

void Blizzard::Update(float deltaTime, ParticleEngine& engine)
{
	spawnCooldown += deltaTime;

	if(spawnCooldown > 0.2f)
	{
		for(size_t i = 0u; i < spawnPoints.size(); ++i)
		{
			Particle particle;
			particle.position = spawnPoints[i];
			particle.acceleration = spawnDirections[i] * spawnVelocity;

			engine.AddParticle(particle);
		}
	}
}
