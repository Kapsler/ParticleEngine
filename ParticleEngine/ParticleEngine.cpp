#include "ParticleEngine.h"
#include "ForceGenerators.hpp"
#include "Config.hpp"

ParticleEngine::ParticleEngine()
{
	//Setting up Particles
	m_particles.reserve(Config::particleCount);

	float particleStep = float(Config::width) / float(Config::particleCount + 1);

	for (size_t i = 0u; i < Config::particleCount; ++i)
	{
		Particle* particle = new Particle();
		particle->position.x = particleStep * (i+1);
		particle->position.y = 0.0f;
		m_particles.push_back(particle);
	}

	//Rendering Stuff
	renderCircle.setRadius(1.0f);
	renderCircle.setOutlineThickness(1.0f);
	renderCircle.setFillColor(sf::Color::Transparent);
	renderCircle.setOutlineColor(sf::Color::Green);
}

ParticleEngine::~ParticleEngine()
{
	for(size_t i = 0u; i < m_particles.size(); ++i)
	{
		delete m_particles[i];
	}

	m_particles.clear();
}

void ParticleEngine::Update(float deltaTime)
{
	for(size_t i = 0u; i < m_particles.size(); ++i)
	{
		ForceGenerators::ApplyGravity(m_particles[i]);
		m_particles[i]->Integrate(deltaTime);
	}
}

void ParticleEngine::Render(sf::RenderWindow& window)
{
	for (size_t i = 0u; i < m_particles.size(); ++i)
	{
		m_particles[i]->Render(window, renderCircle);
	}
}
