#pragma once
#include "Particle.h"
#include "Math.h"

class ParticleForceGenerator
{
public:
	virtual void updateForce(Particle* particle, float t) = 0;
	virtual void PxUpdateForce(Particle* particle, float t) {};

	float distance(Vector3 p1, Vector3 p2);
};

