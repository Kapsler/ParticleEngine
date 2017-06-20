#pragma once
#include <glm/glm.hpp>

namespace Collisions
{
	namespace BoundingVolumes
	{
		struct BoundingBox
		{
			glm::vec2 points[4];
		};

		struct OOBB
		{
			BoundingBox box;
			glm::vec2 u[2]; //X and Y axis
		};

		static void RotateAroundPointDegrees(glm::vec2* points, size_t count, glm::vec2& center, float angle)
		{
			float radians = angle * (3.141592653f / 180.0f);

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
}

