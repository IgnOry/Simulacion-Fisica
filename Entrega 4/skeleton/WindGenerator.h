#pragma once
#include "ParticleForceGenerator.h"
#include "Particle.h"

class WindGenerator: public ParticleForceGenerator
{
	// Acceleration for wind
	Vector3 wind;
	Vector3 pos;
	float radio;
	Particle* particle;
public:
	WindGenerator(const Vector3 wind_, const Vector3 pos_, float radio_);
	virtual void updateForce(Particle* particleEx, float t);
	~WindGenerator();
};

