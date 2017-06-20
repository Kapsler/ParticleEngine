#include "ParticleEngine.h"
#include "ForceGenerators.hpp"
#include "Config.hpp"

ParticleEngine::ParticleEngine()
{
	//Setting up Particles
	m_particles.resize(Config::particleCount);
	m_particleVertices.resize(Config::particleCount);

	float particleStep = float(Config::width) / float(Config::particleCount + 1);

	for (size_t i = 0u; i < Config::particleCount; ++i)
	{
		float posX = particleStep * (i + 1);
		float posY = 0.0f;

		m_particles[i].position.x = posX;
		m_particles[i].position.y = posY;

		m_particleVertices[i].position.x = posX;
		m_particleVertices[i].position.y = posY;
		m_particleVertices[i].color = sf::Color::Green;
	}

	//Setting up Solid geometry
	Solid centerPlatform;
	centerPlatform.SetPosition(sf::Vector2f(Config::width * 0.3f, Config::height * 0.5f));
	centerPlatform.SetSize(sf::Vector2f(Config::width * 0.3f, Config::height * 0.05f));
	m_solids.push_back(centerPlatform);

	Solid floor;
	floor.SetPosition(sf::Vector2f(0.0f, Config::height * 0.99f));
	floor.SetSize(sf::Vector2f(Config::width, Config::height * 0.05f));
	m_solids.push_back(floor);
}

ParticleEngine::~ParticleEngine()
{
	m_particles.clear();
	m_particleVertices.clear();
	m_solids.clear();
}

void ParticleEngine::Update(float deltaTime)
{
	CheckCollisions();
	ApplyForces();
	Integrate(deltaTime);
}

void ParticleEngine::Render(sf::RenderWindow& window)
{
	for (size_t i = 0u; i < m_solids.size(); ++i)
	{
		window.draw(m_solids[i].shape);
	}

	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		m_particleVertices[i].position.x = m_particles[i].position.x;
		m_particleVertices[i].position.y = m_particles[i].position.y;
	}

	window.draw(&m_particleVertices[0], m_particleVertices.size(), sf::PrimitiveType::Points);

}

void ParticleEngine::CheckCollisions()
{
	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		for (size_t j = 0u; j < m_solids.size(); ++j)
		{
			if (m_particles[i].DoesCollideWithAABB(m_solids[j].aabb))
			{
				//Collision with AABB!
				printf("Collision of Particle %zd with Solid %zd.\n\r", i, j);
				m_particles[i].ResolveCollisionWithAABB(m_solids[j].aabb);
			}
		}

		//Check Balls
	}
}

void ParticleEngine::ApplyForces()
{
	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		ForceGenerators::ApplyGravity(m_particles[i]);
	}
}

void ParticleEngine::Integrate(float deltaTime)
{
	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		m_particles[i].Integrate(deltaTime);
	}
}