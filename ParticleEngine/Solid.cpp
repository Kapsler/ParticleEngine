#include "Solid.h"

Solid::Solid()
{
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Blue);
	shape.setOutlineThickness(1.0f);
}

void Solid::Render(sf::RenderWindow& window)
{
	window.draw(shape);
}
