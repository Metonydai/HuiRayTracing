#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	//:0
	alignas(16) glm::vec3 origin;
	alignas(16) glm::vec3 horizontal;
	alignas(16) glm::vec3 vertical;
	alignas(16) glm::vec3 lowerLeftCorner;
	//:4
	float viewportHeight;
	float viewportWidth;
	float aspectRatio;
	float focalLength;
	//:5
	alignas(16) glm::vec3 u;
	alignas(16) glm::vec3 v;
	alignas(16) glm::vec3 w;
	float lensRadius;
public:
	Camera() = default;
	Camera(
		glm::vec3 lookfrom,
		glm::vec3 lookat,
		glm::vec3 vup,
		float vfov, // vertical field-of-view in degrees
		float aspectRatio,
		float aperture,
		float focus_dist);

	Camera(
		const glm::vec3& origin,
		float viewportWidth,
		float viewportHeight,
		float focalLength);

};