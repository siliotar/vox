#include "Frustum.hpp"
#include "Camera.hpp"

Frustum createFrustumFromCamera()
{
	Frustum frustum;
	const float halfVSide = Camera::far * tanf(Camera::fov * 0.5f);
	const float halfHSide = halfVSide * Camera::aspect;
	const glm::vec3 frontMultFar = Camera::far * Camera::getFront();

	frustum.nearFace = { Camera::getPlayerPosition() + Camera::near * Camera::getFront(), Camera::getFront() };
	frustum.farFace = { Camera::getPlayerPosition() + frontMultFar , -Camera::getFront() };
	frustum.rightFace = { Camera::getPlayerPosition(), glm::cross(Camera::getUp(), frontMultFar + Camera::getRight() * halfHSide) };
	frustum.leftFace = { Camera::getPlayerPosition(), glm::cross(frontMultFar - Camera::getRight() * halfHSide, Camera::getUp()) };
	frustum.topFace = { Camera::getPlayerPosition(), glm::cross(Camera::getRight(), frontMultFar - Camera::getUp() * halfVSide) };
	frustum.bottomFace = { Camera::getPlayerPosition(), glm::cross(frontMultFar + Camera::getUp() * halfVSide, Camera::getRight()) };

	return frustum;
}
