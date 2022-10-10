#pragma once

#include "glm/glm.hpp"

struct Plan
{
	glm::vec3 normal = { 0.0f, 1.0f, 0.0f };
	float distance = 0.0f;
	Plan() {}
	Plan(const glm::vec3& p1, const glm::vec3& norm)
		: normal(glm::normalize(norm)), distance(glm::dot(normal, p1)) {}
	float getSignedDistanceToPlan(const glm::vec3& point) const
	{
		return glm::dot(normal, point) - distance;
	}
};

struct Frustum
{
	Plan topFace;
	Plan bottomFace;
	Plan rightFace;
	Plan leftFace;
	Plan farFace;
	Plan nearFace;
};

Frustum createFrustumFromCamera();
