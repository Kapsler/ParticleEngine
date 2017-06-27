#pragma once
#include "Particle.h"
#include <glm/gtx/projection.hpp>

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
		p2.acceleration += force * direction;
	}

	static void ApplyReflexion(Particle& particle, const Collisions::Contact& contact)
	{
		particle.position += (contact.penetration + 0.5f) * contact.contactNormal;

		glm::vec2 relativeAcceleration = -((1.0f + particle.bounciness) * glm::dot(particle.velocity, contact.contactNormal)) * contact.contactNormal;

		particle.acceleration += relativeAcceleration;

		glm::vec2 normalVelocity = glm::proj(particle.velocity, -contact.contactNormal);
		if (Collisions::saveLength(normalVelocity) > 100.0f)
		{
			return;
		}

		glm::vec2 relativeVelocity = glm::vec2(0.0f) - particle.velocity;
		float velocityAlongNormal = glm::dot(relativeVelocity, contact.contactNormal);

		float j = -(1 + particle.bounciness) * velocityAlongNormal;

		relativeVelocity = glm::vec2(0.0f) - particle.velocity;
		glm::vec2 tangent;
		if (relativeVelocity.x * contact.contactNormal.y - relativeVelocity.y * contact.contactNormal.x < 0.0f)
		{
			tangent = glm::vec2(-contact.contactNormal.y, contact.contactNormal.x);
		}
		else
		{
			tangent = glm::vec2(contact.contactNormal.y, -contact.contactNormal.x);
		}

		float jt = -glm::dot(relativeVelocity, tangent);

		// clamp friction and differentiate between static and kinetic friction
		glm::vec2 frictionImpulse;
		if (abs(jt) < j * particle.staticFriction)
		{
			// static friction
			frictionImpulse = jt * tangent;
		}
		else
		{
			// kinematic friction
			frictionImpulse = -j * tangent * particle.kinematicFriction;
		}

		particle.velocity +=/* particle->inverseMass **/ frictionImpulse;
	}

	static void ResolveCollision(Particle& p1, Particle& p2, const Collisions::Contact& contact)
	{
		p1.position += (contact.penetration + 0.5f) * contact.contactNormal;
		p2.position += (contact.penetration + 0.5f) * -contact.contactNormal;

		float normalStrength = p1.velocity.x * contact.contactNormal.x + p1.velocity.y * contact.contactNormal.y;
		normalStrength += p2.velocity.x * -contact.contactNormal.x + p2.velocity.y * -contact.contactNormal.y;
		normalStrength *= 0.5f;

		p1.acceleration -=  2.0f * p1.bounciness * (normalStrength * contact.contactNormal);
		p2.acceleration -=  2.0f * p2.bounciness * (normalStrength * -contact.contactNormal);
	}
}