#pragma once

#include "Frustum.hpp"

struct Volume
{
	virtual bool isOnFrustum(const Frustum& camFrustum) const = 0;
};
