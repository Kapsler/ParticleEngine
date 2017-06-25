#pragma once
#include "Particle.h"
#include <vector>
#include "SFML/Graphics.hpp"
#include "Solid.h"
#include "Blizzard.h"
#include "Ball.h"
#include "Fan.h"
#include "BallGenerator.h"
#include "ForceGenerators.hpp"

class ParticleEngine
{
public:
	ParticleEngine();
	~ParticleEngine();

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void AddParticle(const Particle& particle);
	void AddBall(const Ball& ball);

private:
	void AddSpringContraint(size_t p1Index, size_t p2Index);
	void GenerateCloth(const float ballRadius, const glm::vec2& startPosition, const float spacing);
	void CheckCollisions();
	void ResolveCollisions();
	void ApplyForces();
	void Integrate(float deltaTime);
	void DeleteParticles();

	std::vector<Particle> m_particles;
	std::vector<Solid> m_solids;
	std::vector<Blizzard> m_blizzards;
	std::vector<Collisions::Contact> m_particleContacts;
	std::vector<Collisions::Contact> m_ballContacts;
	std::vector<Ball> m_balls;
	std::vector<Fan> m_fans;
	std::vector<BallGenerator> m_ballGenerators;
	std::vector<ForceGenerators::SpringContraint> m_springs;
	std::vector<Ball> m_cloth;

	//Rendering Stuff
	std::vector<sf::Vertex> m_particleVertices;
	std::vector<sf::Vertex> m_springVertices;
	sf::CircleShape renderCircle;
};
