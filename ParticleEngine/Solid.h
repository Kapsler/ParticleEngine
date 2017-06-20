#pragma once
#include "sfml/Graphics.hpp"

class Solid
{
public:
	Solid();

	void Render(sf::RenderWindow& window);

	sf::RectangleShape shape;
};
