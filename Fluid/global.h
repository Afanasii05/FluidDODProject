#pragma once
namespace global
{
	inline float circleRadius = 1.0f, speedMultiplier = 1.0f;
	inline float maxSpeed = speedMultiplier * 30.0f;
	inline float screenWidth, screenHeight;

	inline int maxCircles = std::floor(200000.0f / circleRadius);

	inline bool mergeEnabled = false;
	inline float mergeOverlap = 50.0f;

	inline bool cursorRepelEnabled = false;
    inline float cursorRepelRadius = 100.0f;
	inline float repelForce = 10.0f;
    inline float mouseX = 0.0f;
    inline float mouseY = 0.0f;
};

