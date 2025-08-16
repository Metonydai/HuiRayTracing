#include "Camera.h"


Camera::Camera(
	glm::vec3 lookfrom,
	glm::vec3 lookat,
	glm::vec3 vup,
	float vfov, // vertical field-of-view in degrees
	float aspectRatio,
	float aperture,
	float focus_dist)
{
	const auto offset = offsetof(Camera, viewportHeight);
	float theta = glm::radians(vfov);
	float h = glm::tan(theta / 2);
	viewportHeight = 2.0f * h;
	viewportWidth = aspectRatio * viewportHeight;
	this->aspectRatio = aspectRatio;

	w = glm::normalize(lookfrom - lookat);
	u = glm::normalize(cross(vup, w));
	v = glm::cross(w, u);

	origin = lookfrom;
	horizontal = focus_dist * viewportWidth * u;
	vertical = focus_dist * viewportHeight * v;
	lowerLeftCorner = origin - horizontal / 2.0f - vertical / 2.0f - w * focus_dist;
	focalLength = focus_dist;
	lensRadius = aperture / 2.0f;
}

Camera::Camera(
	const glm::vec3& origin,
	float viewportWidth,
	float viewportHeight,
	float focalLength)
	: origin(origin), viewportWidth(viewportWidth), viewportHeight(viewportHeight), focalLength(focalLength)
{
	aspectRatio = viewportWidth / viewportHeight;

	u = glm::vec3(1, 0, 0);
	v = glm::vec3(0, 1, 0);
	u = glm::vec3(0, 0, 1);

	lowerLeftCorner = 
		(glm::vec3(0, 0, -focalLength) - viewportWidth / 2 * u - viewportHeight / 2 * v) - origin;

	horizontal = focalLength * viewportWidth * u;
	vertical = focalLength * viewportHeight * v;

	lensRadius = 0.0;;
}

