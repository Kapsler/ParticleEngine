#pragma once
#include "Particle.h"
#include <vector>
#include "SFML/Graphics.hpp"

class ParticleEngine
{
public:
	ParticleEngine();
	~ParticleEngine();

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

private:
	
	std::vector<Particle*> m_particles;

	//Rendering Stuff
	sf::CircleShape renderCircle;
};
