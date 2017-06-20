#pragma once
#include "Particle.h"
#include <vector>
#include "SFML/Graphics.hpp"
#include "Solid.h"

class ParticleEngine
{
public:
	ParticleEngine();
	~ParticleEngine();

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

private:
	void CheckCollisions();
	void ApplyForces();
	void Integrate(float deltaTime);

	std::vector<Particle> m_particles;
	std::vector<Solid> m_solids;

	//Rendering Stuff
	std::vector<sf::Vertex> m_particleVertices;
};
