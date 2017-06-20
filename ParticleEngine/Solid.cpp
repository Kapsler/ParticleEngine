#include "Solid.h"

Solid::Solid()
{
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Blue);
	shape.setOutlineThickness(1.0f);

	sf::FloatRect rect = shape.getGlobalBounds();

	aabb.points[0].x = rect.left;
	aabb.points[0].y = rect.top;
	aabb.points[1].x = rect.left + rect.width;
	aabb.points[1].y = rect.top;
	aabb.points[2].x = rect.left + rect.width;
	aabb.points[2].y = rect.top + rect.height;
	aabb.points[3].x = rect.left;
	aabb.points[3].y = rect.top + rect.height;
}

void Solid::Render(sf::RenderWindow& window)
{
	window.draw(shape);
}

void Solid::SetPosition(const sf::Vector2f& newPos)
{
	shape.setPosition(newPos);

	sf::FloatRect rect = shape.getGlobalBounds();

	aabb.points[0].x = rect.left;
	aabb.points[0].y = rect.top;
	aabb.points[1].x = rect.left + rect.width;
	aabb.points[1].y = rect.top;
	aabb.points[2].x = rect.left + rect.width;
	aabb.points[2].y = rect.top + rect.height;
	aabb.points[3].x = rect.left;
	aabb.points[3].y = rect.top + rect.height;
}

void Solid::SetSize(const sf::Vector2f& newSize)
{
	shape.setSize(newSize);

	sf::FloatRect rect = shape.getGlobalBounds();

	aabb.points[0].x = rect.left;
	aabb.points[0].y = rect.top;
	aabb.points[1].x = rect.left + rect.width;
	aabb.points[1].y = rect.top;
	aabb.points[2].x = rect.left + rect.width;
	aabb.points[2].y = rect.top + rect.height;
	aabb.points[3].x = rect.left;
	aabb.points[3].y = rect.top + rect.height;
}
