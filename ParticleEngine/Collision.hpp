#pragma once
#include <glm/glm.hpp>
#include "Config.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <cstdio>
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

		/*
		 * Orders incoming points the following way
		 * 
		 * points[0] = topmost point
		 * points[1] = rightmost point
		 * points[2] = bottommost point
		 * points[3] = leftmost point
		 * 
		 */
		static void ReorderPoints(glm::vec2* points, const size_t pointCount)
		{
			glm::vec2 smallest, largest;

			if (pointCount <= 0)
			{
				printf("No points given!");
				return;
			}

			smallest = points[0];
			largest = points[0];

			for(size_t i = 0u; i < pointCount; ++i)
			{
				if (points[i].x < smallest.x)
				{
					smallest.x = points[i].x;
				}
				if (points[i].x < largest.x)
				{
					largest.x = points[i].x;
				}
				if (points[i].y < smallest.y)
				{
					smallest.y = points[i].y;
				}
				if (points[i].y > largest.y)
				{
					largest.y = points[i].y;
				}
			}

			glm::vec2 topMost, rightMost, bottomMost, leftMost;

			for (size_t i = 0u; i < pointCount; ++i)
			{
				//TopMost
				if (points[i].y == smallest.y)
				{
					topMost = points[i];
				}
				//RightMost
				if (points[i].x == largest.x)
				{
					rightMost = points[i];
				}
				//BottomMost
				if (points[i].y == largest.y)
				{
					bottomMost = points[i];
				}
				//LeftMost
				if (points[i].x == smallest.x)
				{
					leftMost = points[i];
				}
			}

			points[0].x = topMost.x;
			points[0].y = topMost.y;
			points[1].x = rightMost.x;
			points[1].y = rightMost.y;
			points[2].x = bottomMost.x;
			points[2].y = bottomMost.y;
			points[3].x = leftMost.x;
			points[3].y = leftMost.y;
		}
	}

	struct Contact
	{
		size_t index;
		glm::vec2 contactNormal;
		float penetration;
	};

	static glm::vec2 WorldToLocal(const glm::vec2& worldPoint, const glm::vec2& localTranslation, const glm::vec2& localAxis)
	{
		float angle = glm::acos(glm::dot(glm::vec2(1.0f, 0.0f), localAxis));

		glm::mat3x3 boxMatrix = glm::mat3x3();
		boxMatrix = glm::translate(boxMatrix, localTranslation);
		boxMatrix = glm::rotate(boxMatrix, angle);

		return glm::inverse(boxMatrix) * glm::vec3(worldPoint.x, worldPoint.y, 1);
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

		return squaredDistance <= sphereRadius * sphereRadius;
	}

}

