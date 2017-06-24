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
	shape.setOrigin(shape.getPosition() + (shape.getSize() * 0.5f));

	UpdateBoundingVolumes();
}

void Solid::SetRotation(const float newRotation)
{
	shape.setRotation(newRotation);

	UpdateBoundingVolumes();
}

void Solid::UpdateBoundingVolumes()
{
	//AABB
	auto rect = shape.getGlobalBounds();

	aabb.halfSize.x = rect.width * 0.5f;
	aabb.halfSize.y = rect.height * 0.5f;

	aabb.center.x = rect.left + aabb.halfSize.x;
	aabb.center.y = rect.top + aabb.halfSize.y;

	//OOBB
	glm::vec2 points[4];

	points[0].x = shape.getTransform().transformPoint(shape.getPoint(0)).x;
	points[0].y= shape.getTransform().transformPoint(shape.getPoint(0)).y;
	points[1].x = shape.getTransform().transformPoint(shape.getPoint(1)).x;
	points[1].y = shape.getTransform().transformPoint(shape.getPoint(1)).y;
	points[2].x = shape.getTransform().transformPoint(shape.getPoint(2)).x;
	points[2].y = shape.getTransform().transformPoint(shape.getPoint(2)).y;
	points[3].x = shape.getTransform().transformPoint(shape.getPoint(3)).x;
	points[3].y = shape.getTransform().transformPoint(shape.getPoint(3)).y;

	oobb.box.halfSize.x = shape.getSize().x * 0.5f;
	oobb.box.halfSize.y = shape.getSize().y * 0.5f;

	oobb.box.center.x = points[3].x + aabb.halfSize.x;
	oobb.box.center.y = points[0].y + aabb.halfSize.y;

	oobb.u[0] = glm::normalize(points[1] - points[0]);
	oobb.u[1] = glm::normalize(points[3] - points[0]);
}

void Solid::RenderOOBB(sf::RenderWindow& window)
{
	sf::RectangleShape oobbShape;
	oobbShape.setFillColor(sf::Color::Red);
	oobbShape.setSize(sf::Vector2f(oobb.box.halfSize.x * 2.0f, oobb.box.halfSize.y * 2.0f));
	oobbShape.setOrigin(oobb.box.halfSize.x, oobb.box.halfSize.y);
	oobbShape.setPosition(oobb.box.center.x, oobb.box.center.y);
	
	float angle = glm::acos(glm::dot(glm::vec2(1.0f, 0.0f), oobb.u[0]));

	float degrees = angle * (180.0f / Config::pi);
	oobbShape.rotate(degrees);

	window.draw(oobbShape);
}
