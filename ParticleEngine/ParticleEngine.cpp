#include "ParticleEngine.h"
#include "ForceGenerators.hpp"
#include "Config.hpp"

ParticleEngine::ParticleEngine()
{
	//Setting up Solid geometry
	Solid centerPlatform;
	centerPlatform.SetSize(sf::Vector2f(Config::width * 0.3f, Config::height * 0.02f));
	centerPlatform.SetRotation(45.0f);
	centerPlatform.SetPosition(sf::Vector2f(Config::width * 0.5f, Config::height * 0.5f));
	m_solids.push_back(centerPlatform);

	//Left Wall
	Solid wall; 
	wall.SetSize(sf::Vector2f((float)Config::width* 0.02f, (float)Config::height));
	wall.SetPosition(sf::Vector2f((float)Config::width * 0.0f, (float)Config::height * 0.50f));
	m_solids.push_back(wall);

	//Right Wall
	wall.SetPosition(sf::Vector2f((float)Config::width, (float)Config::height * 0.50f));
	m_solids.push_back(wall);

	Solid floor;
	floor.SetSize(sf::Vector2f((float)Config::width, (float)Config::height * 0.02f));
	floor.SetPosition(sf::Vector2f((float)Config::width * 0.5f, (float)Config::height));
	m_solids.push_back(floor);

	//Setting up blizzards
	Blizzard blizzard1(glm::vec2(Config::width * 0.75f, Config::height * 0.25f), 10);
	m_blizzards.push_back(blizzard1);

	//Setting Up Balls
	Ball ball1;
	ball1.position.x = (float)Config::width	* 0.50f;
	ball1.position.y = (float)Config::height * 0.10f;
	m_balls.push_back(ball1);

	m_particleVertices.reserve(Config::maxParticleCount);
	m_particles.reserve(Config::maxParticleCount);
	m_particleContacts.reserve(Config::maxParticleCount);
	m_ballContacts.reserve(Config::maxBallCount * Config::maxBallCount + Config::maxBallCount * m_solids.size());

	//Setting Up Rendering Stuff
	renderCircle.setPointCount(15);
	renderCircle.setRadius(1.0f);
	renderCircle.setOrigin(renderCircle.getRadius(), renderCircle.getRadius());
	renderCircle.setOutlineThickness(1.0f / m_balls[0].radius);
	renderCircle.setOutlineColor(sf::Color::Yellow);
	renderCircle.setFillColor(sf::Color::Transparent);
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
	DeleteParticles();
}

void ParticleEngine::Render(sf::RenderWindow& window)
{
	for (size_t i = 0u; i < m_solids.size(); ++i)
	{
		m_solids[i].Render(window);
	}

	for (size_t i = 0u; i < m_blizzards.size(); ++i)
	{
		m_blizzards[i].DebugRender(window);
	}

	//Balls
	for (size_t i = 0u; i < m_balls.size(); ++i)
	{
		renderCircle.setPosition(m_balls[i].position.x, m_balls[i].position.y);
		renderCircle.setScale(m_balls[i].radius, m_balls[i].radius);
		window.draw(renderCircle);
	}

	//Particles
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
				if(Collisions::PointBoxCollision(m_solids[j].oobb, m_particles[i].position, contact))
				{
					//OOBB Collision!
					contact.index = i;
					m_particleContacts.push_back(contact);
				}
			}
		}

		//Check Balls
		for (size_t j = 0u; j < m_balls.size(); ++j)
		{
			if(m_particles[i].DoesCollideWithSphere(m_balls[j].position, m_balls[j].radius + 1.0f))
			{
				m_particles[i].toBeDeleted = true;
			}
		}
	}
	

	for (size_t i = 0u; i < m_balls.size(); ++i)
	{
		for (size_t j = 0u; j < m_solids.size(); ++j)
		{
			if (m_balls[i].DoesCollideWithAABB(m_solids[j].aabb))
			{
				if(Collisions::SphereBoxCollision(m_balls[i].position, m_balls[i].radius, m_solids[j].oobb, contact))
				{
					contact.index = i;
					m_ballContacts.push_back(contact);
				}
			}
		}
	}

}

void ParticleEngine::ApplyForces()
{
	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		ForceGenerators::ApplyGravity(m_particles[i]);
	}
	for (size_t i = 0u; i < m_balls.size(); ++i)
	{
		ForceGenerators::ApplyGravity(m_balls[i]);
	}
}

void ParticleEngine::Integrate(float deltaTime)
{
	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		m_particles[i].Integrate(deltaTime);
	}
	for (size_t i = 0u; i < m_balls.size(); ++i)
	{
		m_balls[i].Integrate(deltaTime);
	}
}

void ParticleEngine::DeleteParticles()
{
	std::vector<Particle>::iterator it = m_particles.begin();

	while(it != m_particles.end())
	{
		if(it->toBeDeleted)
		{
			it = m_particles.erase(it);
		} else
		{
			++it;
		}
	}	
}

void ParticleEngine::ResolveCollisions()
{
	for(size_t i = 0u; i < m_particleContacts.size(); ++i)
	{
		m_particles[m_particleContacts[i].index].ResolveCollision(m_particleContacts[i]);
	}

	m_particleContacts.clear();

	for (size_t i = 0u; i < m_ballContacts.size(); ++i)
	{
		m_balls[m_ballContacts[i].index].ResolveCollision(m_ballContacts[i]);
	}

	m_ballContacts.clear();

}
