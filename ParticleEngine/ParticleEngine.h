#pragma once
#include "Particle.h"
#include <vector>
#include "SFML/Graphics.hpp"
#include "Solid.h"
#include "Blizzard.h"
#include "Ball.h"

class ParticleEngine
{
public:
	ParticleEngine();
	~ParticleEngine();

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void AddParticle(const Particle& particle);

private:
	void CheckCollisions();
	void ResolveCollisions();
	void ApplyForces();
	void Integrate(float deltaTime);

	std::vector<Particle> m_particles;
	std::vector<Solid> m_solids;
	std::vector<Blizzard> m_blizzards;
	std::vector<Collisions::Contact> m_particleContacts;
	std::vector<Collisions::Contact> m_ballContacts;
	std::vector<Ball> m_balls;

	//Rendering Stuff
	std::vector<sf::Vertex> m_particleVertices;
	sf::CircleShape renderCircle;
};
