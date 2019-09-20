#pragma once

#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>

using namespace physx;

class Particle
{
public:
	Particle(float radius, Vector4 c = Vector4(0.5, 0.5, 0.5, 1), Vector3 position = Vector3(0.0f, 0.0f, 0.0f));
	void integrate(float t);

protected:
	float damping = 1.0f;
	float radius;
	float inverseMass = 45.f;
	
	Vector3 acceleration = Vector3(1.0,0.0,0.0);
	Vector3 velocity = Vector3(1.0,0,0);
	Vector3 position;

	Vector4 color;

	PxTransform* transform;

	RenderItem* rItem;
};

