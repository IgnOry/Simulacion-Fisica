#pragma once

#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>

using namespace physx;

class Particle
{
public:
	Particle(float radius, Vector4 c = Vector4(0.5, 0.5, 0.5, 1), Vector3 position = Vector3(0.0f, 0.0f, 0.0f));
protected:
	RenderItem* rItem;
};

