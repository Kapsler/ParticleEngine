#pragma once
#include <glm/glm.hpp>
#include "Config.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/norm.hpp>
#define GLM_FORCE_RADIANS

namespace Collisions
{
	namespace BoundingVolumes
	{
		struct AABB
		{
			glm::vec2 min;
			glm::vec2 max;
		};

		struct OOBB
		{
			glm::vec2 center;
			glm::vec2 halfSize;
			glm::vec2 u[2]; //X and Y axis
		};

		static void RotateAroundPointDegrees(glm::vec2* points, size_t count, glm::vec2& center, float angle)
		{
			float radians = angle * (Config::pi / 180.0f);

			for (size_t i = 0u; i < count; ++i)
			{
				float Tx = points[i].x - center.x;
				float Ty = points[i].y - center.y;

				points[i].x = (Tx*cosf(radians)) - (Ty*sinf(radians)) + center.x;
				points[i].y = (Ty*cosf(radians)) + (Tx*sinf(radians)) + center.y;
			}
		}


		static void RotateAroundPointRads(glm::vec2* points, size_t count, glm::vec2& center, float angle)
		{
			float sinA = sinf(angle);
			float cosA = cosf(angle);

			for (size_t i = 0u; i < count; ++i)
			{
				float Tx = points[i].x - center.x;
				float Ty = points[i].y - center.y;

				points[i].x = (Tx*cosA) - (Ty*sinA) + center.x;
				points[i].y = (Ty*cosA) + (Tx*sinA) + center.y;
			}
		}
	}

	struct Contact
	{
		size_t index;
		glm::vec2 contactNormal;
		float penetration;
	};

	static glm::vec2 saveNormalize(const glm::vec2& vector)
	{
		glm::vec2 normal = glm::normalize(vector);

		if( std::isnan(normal.x) || std::isnan(normal.y))
		{
			normal.x = 0.0f;
			normal.y = 0.0f;
		}

		return normal;
	}

	static float saveLength(const glm::vec2& vector)
	{
		float length = glm::length(vector);

		if (std::isnan(length))
		{
			length = 0.0f;
		}

		return length;
	}

	static float saveDistance(const glm::vec2& p1, const glm::vec2& p2)
	{
		float distance = glm::distance(p1, p2);

		if (std::isnan(distance))
		{
			distance = 0.0f;
		}

		return distance;
	}

	static glm::vec2 WorldToLocal(const glm::vec2& worldPoint, const glm::vec2& localTranslation, const glm::vec2& localAxis)
	{
		float angle = glm::acos(glm::dot(glm::vec2(1.0f, 0.0f), localAxis));

		glm::mat3x3 boxMatrix = glm::mat3x3();
		boxMatrix = glm::translate(boxMatrix, localTranslation);
		boxMatrix = glm::rotate(boxMatrix, angle);

		return glm::inverse(boxMatrix) * glm::vec3(worldPoint.x, worldPoint.y, 1);
	}

	static glm::vec2 LocalToWorld(const glm::vec2& localPoint, const glm::vec2& localTranslation, const glm::vec2& localAxis)
	{
		float angle = glm::acos(glm::dot(glm::vec2(1.0f, 0.0f), localAxis));

		glm::mat3x3 boxMatrix = glm::mat3x3();
		boxMatrix = glm::translate(boxMatrix, localTranslation);
		boxMatrix = glm::rotate(boxMatrix, angle);

		return boxMatrix * glm::vec3(localPoint.x, localPoint.y, 1);
	}

	static bool PointBoxCollision(const glm::vec2& point, const BoundingVolumes::AABB& aabb)
	{
		if (point.x > aabb.min.x && point.x < aabb.max.x
			&&	point.y > aabb.min.y && point.y < aabb.max.y)
		{
			return true;
		}

		return false;
	}

	static bool PointSphereCollision(const glm::vec2& point, const glm::vec2& sphereCenter, const float sphereRadius)
	{
		return saveDistance(sphereCenter, point) < sphereRadius;
	}

	static bool PointBoxCollision(const BoundingVolumes::OOBB& oobb, const glm::vec2& point, Contact& contact)
	{
		glm::vec2 relativePoint = WorldToLocal(point, oobb.center, oobb.u[0]);

		//Check axis where penetration is least deep
		float minDepth = oobb.halfSize.x - abs(relativePoint.x);
		if (minDepth < 0)
		{
			return false;
		}
		glm::vec2 normal = oobb.u[0] * (relativePoint.x < 0 ? -1.0f : 1.0f);

		float depth = oobb.halfSize.y - abs(relativePoint.y);
		if(depth < 0)
		{
			return false;
		}
		if(depth < minDepth)
		{
			minDepth = depth;
			normal = oobb.u[1] * (relativePoint.y < 0 ? -1.0f : 1.0f);
		}

		//Set Collision Data
		contact.contactNormal = normal;
		contact.penetration = minDepth;

		return true;

	}
	
	static float SquaredDistancePointToAABB(const glm::vec2& point, const BoundingVolumes::AABB& aabb)
	{
		auto check = [&](
			const float pn,
			const float bmin,
			const float bmax) -> float
		{
			float out = 0;
			float v = pn;

			if (v < bmin)
			{
				float val = (bmin - v);
				out += val * val;
			}

			if (v > bmax)
			{
				float val = (v - bmax);
				out += val * val;
			}

			return out;
		};

		float squaredDistance = 0.0f;

		squaredDistance += check(point.x, aabb.min.x, aabb.max.x);
		squaredDistance += check(point.y, aabb.min.y, aabb.max.y);

		return squaredDistance;
	}

	static bool SphereBoxCollision(const glm::vec2& sphereCenter, const float sphereRadius, const BoundingVolumes::AABB& aabb)
	{
		float squaredDistance = SquaredDistancePointToAABB(sphereCenter, aabb);

		return squaredDistance < sphereRadius * sphereRadius;
	}

	static bool SphereBoxCollision(const glm::vec2& sphereCenter, const float sphereRadius, const BoundingVolumes::OOBB& oobb, Contact& contact)
	{
		glm::vec2 relCenter = WorldToLocal(sphereCenter, oobb.center, oobb.u[0]);

		//Early Exits
		if(		abs(relCenter.x) - sphereRadius > oobb.halfSize.x
			||	abs(relCenter.y) - sphereRadius > oobb.halfSize.y)
		{
			return false;
		}

		glm::vec2 closestPoint;
		float distance;

		distance = relCenter.x;
		if (distance > oobb.halfSize.x) distance = oobb.halfSize.x;
		if (distance < -oobb.halfSize.x) distance = -oobb.halfSize.x;
		closestPoint.x = distance;

		distance = relCenter.y;
		if (distance > oobb.halfSize.y) distance = oobb.halfSize.y;
		if (distance < -oobb.halfSize.y) distance = -oobb.halfSize.y;
		closestPoint.y = distance;

		//Contact Check
		distance = saveDistance(closestPoint, relCenter);
		if (distance < 0 || distance > sphereRadius)
		{
			return false;
		}

		//Setup contact data
		closestPoint = LocalToWorld(closestPoint, oobb.center, oobb.u[0]);

		contact.contactNormal = saveNormalize(sphereCenter - closestPoint);
		contact.penetration = sphereRadius - distance;

		return true;
	}

	static bool SphereSphereCollision(const glm::vec2& sphere1Center, const float sphere1Radius, const glm::vec2& sphere2Center, const float sphere2Radius, Contact& contact)
	{
		glm::vec2 midline = (sphere2Center - sphere1Center);
		float distance = saveLength(midline);

		if(distance < 0.0f || distance > sphere1Radius + sphere2Radius)
		{
			return false;
		}
		
		contact.contactNormal = -saveNormalize(midline);
		contact.penetration = (sphere1Radius + sphere2Radius) - distance;

		return true;
	}

	
}

