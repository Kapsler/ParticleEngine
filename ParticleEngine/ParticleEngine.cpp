#include "ParticleEngine.h"
#include "ForceGenerators.hpp"
#include "Config.hpp"

ParticleEngine::ParticleEngine()
{
	m_particleVertices.reserve(Config::maxParticleCount);
	m_particles.reserve(Config::maxParticleCount);
	m_contacts.reserve(Config::maxParticleCount);
	////Setting up Particles
	//m_particles.resize(Config::maxParticleCount);
	//m_particleVertices.resize(Config::maxParticleCount);

	//float particleStep = float(Config::width) / float(Config::maxParticleCount + 1);

	//for (size_t i = 0u; i < Config::maxParticleCount; ++i)
	//{
	//	float posX = particleStep * (i + 1);
	//	float posY = 0.0f;

	//	m_particles[i].position.x = posX;
	//	m_particles[i].position.y = posY;

	//	m_particleVertices[i].position.x = posX;
	//	m_particleVertices[i].position.y = posY;
	//	m_particleVertices[i].color = sf::Color::Green;
	//}

	//Setting up Solid geometry
	Solid centerPlatform;
	centerPlatform.SetPosition(sf::Vector2f(Config::width * 0.3f, Config::height * 0.5f));
	centerPlatform.SetSize(sf::Vector2f(Config::width * 0.3f, Config::height * 0.05f));
	m_solids.push_back(centerPlatform);

	Solid floor;
	floor.SetPosition(sf::Vector2f(20.0f, Config::height * 0.90f));
	floor.SetSize(sf::Vector2f(Config::width, Config::height * 0.05f));
	m_solids.push_back(floor);

	//Setting up blizzards
	Blizzard b1(glm::vec2(Config::width * 0.75f, Config::height * 0.25f), 10);
	m_blizzards.push_back(b1);
}

ParticleEngine::~ParticleEngine()
{
	m_particles.clear();
	m_particleVertices.clear();
	m_solids.clear();
}

void ParticleEngine::Update(float deltaTime)
{
	for(size_t i = 0u; i < m_blizzards.size(); ++i)
	{
		m_blizzards[i].Update(deltaTime, *this);
	}

	ApplyForces();
	Integrate(deltaTime);
	CheckCollisions();
	ResolveCollisions();
	Integrate(deltaTime);
}

void ParticleEngine::Render(sf::RenderWindow& window)
{
	for (size_t i = 0u; i < m_solids.size(); ++i)
	{
		window.draw(m_solids[i].shape);
	}

	for (size_t i = 0u; i < m_blizzards.size(); ++i)
	{
		m_blizzards[i].DebugRender(window);
	}

	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		m_particleVertices[i].position.x = m_particles[i].position.x;
		m_particleVertices[i].position.y = m_particles[i].position.y;
	}

	if(!m_particleVertices.empty())
	{
		window.draw(&m_particleVertices[0], m_particleVertices.size(), sf::PrimitiveType::Points);
	}
}

void ParticleEngine::AddParticle(const Particle& particle)
{
	if(m_particles.size() + 1 > Config::maxParticleCount)
	{
		m_particles.erase(m_particles.begin());
	}
	if (m_particleVertices.size() + 1 > Config::maxParticleCount)
	{
		m_particleVertices.erase(m_particleVertices.begin());
	}

	m_particles.push_back(particle);

	sf::Vertex vertex;
	vertex.position.x = particle.position.x;
	vertex.position.y = particle.position.y;
	vertex.color = sf::Color::Green;

	m_particleVertices.push_back(vertex);
}

void ParticleEngine::CheckCollisions()
{
	Collisions::Contact contact;

	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		for (size_t j = 0u; j < m_solids.size(); ++j)
		{
			if (m_particles[i].DoesCollideWithAABB(m_solids[j].aabb))
			{
				//Collision with AABB!
				//printf("Collision of Particle %zd with Solid %zd.\n\r", i, j);
				if(Collisions::PointBoxCollision(m_solids[j].oobb, m_particles[i].position, contact))
				{
					contact.particleIndex = i;
					m_contacts.push_back(contact);
				}
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

void ParticleEngine::ResolveCollisions()
{
	for(size_t i = 0u; i < m_contacts.size(); ++i)
	{
		m_particles[m_contacts[i].particleIndex].ResolveCollision(m_contacts[i]);
	}

	m_contacts.clear();
}
