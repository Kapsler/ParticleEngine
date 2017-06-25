#include "ParticleEngine.h"
#include "ForceGenerators.hpp"
#include "Config.hpp"

ParticleEngine::ParticleEngine()
{
	//Setting up Solid geometry
	Solid centerPlatform;
	centerPlatform.SetSize(sf::Vector2f((float)Config::width * 0.3f, (float)Config::height * 0.05f));
	centerPlatform.SetRotation(15.0f);
	centerPlatform.SetPosition(sf::Vector2f((float)Config::width * 0.5f, (float)Config::height * 0.5f));
	m_solids.push_back(centerPlatform);

	//BottemLeft Platform
	Solid leftBottomPlatform;
	leftBottomPlatform.SetSize(sf::Vector2f((float)Config::width * 0.3f, (float)Config::height * 0.4f));
	leftBottomPlatform.SetRotation(45.0f);
	leftBottomPlatform.SetPosition(sf::Vector2f((float)Config::width * 0.0f, (float)Config::height));
	m_solids.push_back(leftBottomPlatform);

	//Left Wall
	Solid wall; 
	wall.SetSize(sf::Vector2f((float)Config::width* 0.45f, (float)Config::height));
	wall.SetPosition(sf::Vector2f((float)Config::width * -0.2f, (float)Config::height * 0.50f));
	m_solids.push_back(wall);

	//Right Wall
	wall.SetPosition(sf::Vector2f((float)Config::width * 1.2f, (float)Config::height * 0.50f));
	m_solids.push_back(wall);

	//floor
	Solid floor;
	floor.SetSize(sf::Vector2f((float)Config::width, (float)Config::height * 0.45f));
	floor.SetPosition(sf::Vector2f((float)Config::width * 0.5f, (float)Config::height * 1.2f));
	m_solids.push_back(floor);

	//ceiling
	floor.SetPosition(sf::Vector2f((float)Config::width * 0.5f, (float)Config::height * -0.2f));
	m_solids.push_back(floor);

	//Setting up blizzards
	Blizzard blizzard1(glm::vec2((float)Config::width * 0.75f, (float)Config::height * 0.25f), 20);
	m_blizzards.push_back(blizzard1);

	Blizzard blizzard2(glm::vec2((float)Config::width * 0.25f, (float)Config::height * 0.25f), 5);
	m_blizzards.push_back(blizzard2);

	//Setting Up BallGenerator
	BallGenerator ballGen1(glm::vec2((float)Config::width * 0.25f, (float)Config::height * 0.15f), glm::vec2((float)Config::width * 0.75f, (float)Config::height * 0.15f), 10.0f);
	m_ballGenerators.push_back(ballGen1);

	//Setting up Fans
	Fan fan1(glm::vec2((float)Config::width * 0.95f, (float)Config::height * 0.15f), glm::vec2((float)Config::width * 0.95f, (float)Config::height * 0.35f), 10.0f);
	m_fans.push_back(fan1);

	Fan fan2(glm::vec2((float)Config::width * 0.95f, (float)Config::height * 0.95f), glm::vec2((float)Config::width * 0.75f, (float)Config::height * 0.95f), 20.0f);
	m_fans.push_back(fan2);

	m_particleVertices.reserve(Config::maxParticleCount);
	m_particles.reserve(Config::maxParticleCount);
	m_particleContacts.reserve(Config::maxParticleCount);
	m_ballContacts.reserve(Config::maxBallCount * Config::maxBallCount + Config::maxBallCount * m_solids.size());

	//Setting Up Rendering Stuff
	renderCircle.setPointCount(15);
	renderCircle.setRadius(1.0f);
	renderCircle.setOrigin(renderCircle.getRadius(), renderCircle.getRadius());
	renderCircle.setOutlineThickness(1.0f / Config::ballSize);
	renderCircle.setOutlineColor(sf::Color::Yellow);
	renderCircle.setFillColor(sf::Color::Transparent);
}

ParticleEngine::~ParticleEngine()
{
}

void ParticleEngine::Update(float deltaTime)
{
	for (size_t i = 0u; i < m_blizzards.size(); ++i)
	{
		m_blizzards[i].Update(deltaTime, *this);
	}

	for (size_t i = 0u; i < m_ballGenerators.size(); ++i)
	{
		m_ballGenerators[i].Update(deltaTime, *this);
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

	for (size_t i = 0u; i < m_fans.size(); ++i)
	{
		m_fans[i].Render(window);
	}

	for (size_t i = 0u; i < m_ballGenerators.size(); ++i)
	{
		m_ballGenerators[i].Render(window);
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
	vertex.color = sf::Color::White;

	m_particleVertices.push_back(vertex);
}

void ParticleEngine::AddBall(const Ball& ball)
{
	if (m_balls.size() + 1 > Config::maxBallCount)
	{
		m_balls.erase(m_balls.begin());
	}

	m_balls.push_back(ball);
}

void ParticleEngine::CheckCollisions()
{
	Collisions::Contact contact;

	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		for (size_t j = 0u; j < m_solids.size(); ++j)
		{
			if (Collisions::PointBoxCollision(m_particles[i].position, m_solids[j].aabb))
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
			if(Collisions::PointSphereCollision(m_particles[i].position, m_balls[j].position, m_balls[j].radius))
			{
				m_particles[i].toBeDeleted = true;
			}
		}

		for (size_t j = 0u; j < m_fans.size(); ++j)
		{
			m_fans[j].InfluenceParticle(m_particles[i]);
		}
	}
	

	for (size_t i = 0u; i < m_balls.size(); ++i)
	{
		//Solids
		for (size_t j = 0u; j < m_solids.size(); ++j)
		{
			if (Collisions::SphereBoxCollision(m_balls[i].position, m_balls[i].radius, m_solids[j].aabb))
			{
				if(Collisions::SphereBoxCollision(m_balls[i].position, m_balls[i].radius, m_solids[j].oobb, contact))
				{
					contact.index = i;
					m_ballContacts.push_back(contact);
				}
			}
		}

		//Collision Between balls O(N * log(N))
		for (size_t j = i+1; j < m_balls.size(); ++j)
		{
			if (Collisions::SphereSphereCollision(m_balls[i].position, m_balls[i].radius, m_balls[j].position, m_balls[j].radius, contact))
			{
				contact.index = i;
				m_ballContacts.push_back(contact);
				contact.index = j;
				contact.contactNormal *= -1.0f;
				m_ballContacts.push_back(contact);
			}
		}

		//Fans
		for (size_t j = 0u; j < m_fans.size(); ++j)
		{
			m_fans[j].InfluenceParticle(m_balls[i]);
		}
	}

}

void ParticleEngine::ApplyForces()
{
	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		ForceGenerators::ApplyGravity(m_particles[i]);
		ForceGenerators::ApplyAirDrag(m_particles[i]);
	}
	for (size_t i = 0u; i < m_balls.size(); ++i)
	{
		ForceGenerators::ApplyGravity(m_balls[i]);
		ForceGenerators::ApplyAirDrag(m_balls[i]);
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
	std::vector<sf::Vertex>::iterator vit = m_particleVertices.begin();

	while(it != m_particles.end())
	{
		if(it->toBeDeleted)
		{
			it = m_particles.erase(it);
			vit = m_particleVertices.erase(vit);
		} else
		{
			++it;
			++vit;
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
