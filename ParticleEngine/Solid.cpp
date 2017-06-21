#include "Solid.h"

Solid::Solid()
{
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Blue);
	shape.setOutlineThickness(1.0f);

	sf::FloatRect rect = shape.getGlobalBounds();

	UpdateBoundingVolumes();
}

void Solid::Render(sf::RenderWindow& window)
{
	window.draw(shape);
}



void Solid::SetPosition(const sf::Vector2f& newPos)
{
	shape.setPosition(newPos);

	UpdateBoundingVolumes();
}

void Solid::SetSize(const sf::Vector2f& newSize)
{
	shape.setSize(newSize);

	UpdateBoundingVolumes();
}

void Solid::UpdateBoundingVolumes()
{
	const sf::FloatRect& rect = shape.getGlobalBounds();

	aabb.halfSize.x = rect.width * 0.5f;
	aabb.halfSize.y = rect.height * 0.5f;

	aabb.center.x = rect.left + aabb.halfSize.x;
	aabb.center.y = rect.top + aabb.halfSize.y;

	oobb.box = aabb;
	oobb.u[0] = glm::vec2(1.0f, 0.0f);
	oobb.u[1] = glm::vec2(0.0f, 1.0f);
}
