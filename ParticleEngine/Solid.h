#pragma once
#include "sfml/Graphics.hpp"
#include "Collision.hpp"

struct Solid
{
	Solid();

	void Render(sf::RenderWindow& window);
	void SetPosition(const sf::Vector2f& newPos);
	void SetSize(const sf::Vector2f& newSize);
	void SetRotation(const float newRotation);

	sf::RectangleShape shape;

	Collisions::BoundingVolumes::AABB aabb;
	Collisions::BoundingVolumes::OOBB oobb;

private:
	void UpdateBoundingVolumes();
	void RenderOOBB(sf::RenderWindow& window);
};
