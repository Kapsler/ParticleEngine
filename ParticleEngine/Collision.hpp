#pragma once
#include <glm/glm.hpp>
#include "Config.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#define GLM_FORCE_RADIANS

namespace Collisions
{
	namespace BoundingVolumes
	{
		struct BoundingBox
		{
			glm::vec2 center;
			glm::vec2 halfSize;
		};

		struct OOBB
		{
			BoundingBox box;
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
		size_t particleIndex;
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
		glm::vec2 relativePoint = WorldToLocal(point, oobb.box.center, oobb.u[0]);


		//Check axis where penetration is least deep
		float minDepth = oobb.box.halfSize.x - abs(relativePoint.x);
		if (minDepth < 0)
		{
			return false;
		}
		glm::vec2 normal = oobb.u[0] * (relativePoint.x < 0 ? -1.0f : 1.0f);

		float depth = oobb.box.halfSize.y - abs(relativePoint.y);
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
}

