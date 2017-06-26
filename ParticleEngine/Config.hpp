#pragma once

namespace Config
{
	const static unsigned int width = 1600;
	const static unsigned int height = 900;
	const static size_t maxParticleCount = 100000;
	const static size_t maxBallCount = 15;
	const static size_t clothColumns = 7;
	const static size_t clothRows = 7;
	const static float ballSize = 10.0f;
	const static float physicFactor = 5.0f;
	const static float pi = 3.14159265358979f;
	const static bool useVsync = true;
	const static bool useFixedUpdate = false;
	const static float fixedPhysicsUpdate = 1.0f / 60.0f;
}
