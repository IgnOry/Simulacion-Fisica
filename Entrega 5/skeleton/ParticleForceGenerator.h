#pragma once
#include "Particle.h"
#include "Math.h"

class ParticleForceGenerator
{
public:
	virtual void updateForce(Particle* particle, float t) = 0;

	float distance(Vector3 p1, Vector3 p2);
};

