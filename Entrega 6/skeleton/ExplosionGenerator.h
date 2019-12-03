#pragma once
#include "ParticleForceGenerator.h"
#include "Particle.h"

class ExplosionGenerator : public ParticleForceGenerator
{
	Vector3 pos;
	float radio;
	Particle* particle;
	float force;
public:
	ExplosionGenerator(const Vector3 pos_, float radio_);
	~ExplosionGenerator();
	virtual void updateForce(Particle* particleEx, float t);
	virtual void PxUpdateForce(Particle* particleEx, float t);
};
