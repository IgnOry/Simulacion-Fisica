#pragma once
#include "ParticleForceGenerator.h"

class ParticleBungee : public ParticleForceGenerator
{
	// The other particle
	Particle* other;
	// Elasticity constant
	float k;
	float restLength;
public:
	ParticleBungee(Particle* other, float _k, float _restLength);
	virtual void updateForce(Particle* particle, float t);
};

