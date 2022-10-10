#include "AABB.hpp"

bool AABB::isOnFrustum(const Frustum& camFrustum) const
{
	return isOnOrForwardPlan(camFrustum.leftFace) && isOnOrForwardPlan(camFrustum.rightFace) && \
		isOnOrForwardPlan(camFrustum.topFace) && isOnOrForwardPlan(camFrustum.bottomFace) && \
		isOnOrForwardPlan(camFrustum.nearFace) && isOnOrForwardPlan(camFrustum.farFace);
}

bool AABB::isOnOrForwardPlan(const Plan& plan) const
{
	const float r = extents.x * std::abs(plan.normal.x) + \
		extents.y * std::abs(plan.normal.y) + \
		extents.z * std::abs(plan.normal.z);

	return -r <= plan.getSignedDistanceToPlan(center);
}
