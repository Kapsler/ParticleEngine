#pragma once
#include "Particle.h"

namespace ForceGenerators
{
	struct ParticleCollision
	{
		Particle* p1;
		Particle* p2;
		Collisions::Contact contact;
	};

	const static glm::vec2 g_gravity(0.0f, 9.81f);
	const static float g_airPressure = 0.99f;

	struct SpringContraint
	{
		Particle* p1;
		Particle* p2;
		float restLength;
		float stiffness;
	};

	static void ApplyGravity(Particle& particle)
	{
		particle.acceleration += g_gravity;
	}

	static void ApplyAirDrag(Particle& particle)
	{
		particle.velocity *= g_airPressure;
	}

	static void ApplySpringForces(Particle& p1, Particle& p2, float restLength, float stiffness)
	{
		glm::vec2 direction = Collisions::saveNormalize(p2.position - p1.position);
		float currentLength = Collisions::saveDistance(p1.position, p2.position);

		float force = -stiffness * (currentLength - restLength);

		p1.acceleration += force * -direction;
		p2.acceleration += -force * -direction;
	}

	static void ApplyReflexion(Particle& particle, const Collisions::Contact& contact)
	{
		particle.position += (contact.penetration + 0.5f) * contact.contactNormal;

		particle.acceleration += -((1.0f + particle.bouncyness) * glm::dot(particle.velocity, contact.contactNormal)) * contact.contactNormal;

	}

	static void ResolveCollision(Particle& p1, Particle& p2, const Collisions::Contact& contact)
	{
		p1.position += (contact.penetration + 0.5f) * contact.contactNormal;
		p2.position += (contact.penetration + 0.5f) * -contact.contactNormal;

		float normalStrength = p1.velocity.x * contact.contactNormal.x + p1.velocity.y * contact.contactNormal.y;
		normalStrength += p2.velocity.x * -contact.contactNormal.x + p2.velocity.y * -contact.contactNormal.y;
		normalStrength *= 0.5f;

		p1.acceleration -=  2.0f * p1.bouncyness * (normalStrength * contact.contactNormal);
		p2.acceleration -=  2.0f * p2.bouncyness * (normalStrength * -contact.contactNormal);
	}
}