#pragma once

#include "Volume.hpp"

struct AABB : public Volume
{
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	glm::vec3 extents = { 0.0f, 0.0f, 0.0f };

	AABB(const glm::vec3& min, const glm::vec3& max)
		: center((max + min) * 0.5f),
		extents(max.x - center.x, max.y - center.y, max.z - center.z) {}

	bool isOnFrustum(const Frustum& camFrustum) const final;
	bool isOnOrForwardPlan(const Plan& plan) const;
};
