#pragma once
#include "ParticleForceGenerator.h"

class ParticleSpring : public ParticleForceGenerator
{
	// The other particle the spring is attached to
	Particle* other;
	// Elasticity constant
	float k;
	float restLength;
public:
	ParticleSpring(float k_, float restLength_, Vector3 pos);
	~ParticleSpring();
	virtual void updateForce(Particle* particle, float t);
	void updateK(float k_);
	float getK();
	void kkk(float k_);
	void setPosition(Vector3 pos);
	Particle* getOther();
};
