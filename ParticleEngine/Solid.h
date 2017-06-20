#pragma once
#include "sfml/Graphics.hpp"
#include "Collision.hpp"

struct Solid
{
	Solid();

	void Render(sf::RenderWindow& window);
	void SetPosition(const sf::Vector2f& newPos);
	void SetSize(const sf::Vector2f& newSize);

	sf::RectangleShape shape;

	Collisions::BoundingVolumes::BoundingBox aabb;
	Collisions::BoundingVolumes::OOBB oobb;
};
