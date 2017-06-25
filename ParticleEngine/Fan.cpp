#include "Fan.h"

Fan::Fan()
{
	renderPoints.resize(0);
	renderPoints.setPrimitiveType(sf::PrimitiveType::Lines);
	points[0] = glm::vec2();
	points[1] = glm::vec2();
	fanVec = glm::vec2();
	blowDirection = glm::vec2();
}

Fan::Fan(const glm::vec2& start, const glm::vec2& end, float strength)
	: strength(strength)
{
	points[0] = start;
	points[1] = end;


	fanVec = end - start;
	blowDirection.x = -fanVec.y;
	blowDirection.y = fanVec.x;
	blowDirection = glm::normalize(blowDirection);

	renderPoints.setPrimitiveType(sf::PrimitiveType::Lines);
	renderPoints.resize(4);

	renderPoints[0].position.x = start.x;
	renderPoints[0].position.y = start.y;
	renderPoints[0].color = sf::Color::Green;
	renderPoints[1].position.x = end.x;
	renderPoints[1].position.y = end.y;
	renderPoints[1].color = sf::Color::Green;
	renderPoints[2].position.x = (start + end).x * 0.5f;
	renderPoints[2].position.y = (start + end).y * 0.5f;
	renderPoints[2].color = sf::Color::Green;
	renderPoints[3].position.x = renderPoints[2].position.x + (blowDirection.x * strength);
	renderPoints[3].position.y = renderPoints[2].position.y + (blowDirection.y * strength);
	renderPoints[3].color = sf::Color::Green;
}

Fan::Fan(const Fan& other): strength(other.strength)
{
	points[0] = other.points[0];
	points[1] = other.points[1];

	renderPoints = other.renderPoints;
	fanVec = other.fanVec;
	blowDirection = other.blowDirection;
}

Fan::~Fan()
{

}

void Fan::InfluenceParticle(Particle& particle)
{
	float position = glm::sign((points[1].x - points[0].x) * (particle.position.y - points[0].y) - (points[1].y - points[0].y) * (particle.position.x - points[0].x));

	//If left of line
	if(position > 0.0f)
	{
		float projection = glm::dot(fanVec, particle.position - points[0]);

		//If projected point is on line
		if(projection > 0 && projection < glm::dot(fanVec, fanVec))
		{
			particle.acceleration += blowDirection * strength;
		}
	}
}

void Fan::Render(sf::RenderWindow& window)
{
	window.draw(renderPoints);
}
