#include "ParticleSpring.h"

ParticleSpring::ParticleSpring(float k_, float restLength_)
{
	other = new Particle();
	other->setLifeTime(50.0f);
	other->setRItem(1);

	k = k_;
	restLength = restLength_;
}

ParticleSpring::~ParticleSpring()
{
	delete other;
}

void ParticleSpring::updateForce(Particle* particle, float t)
{
	// Calculate distance vector
	Vector3 f = particle->getPosition();
	f -= other->getPosition();
	// Length
	float length = f.normalize();
	// Resulting force
	float deltaL = (length - restLength);
	float forceMagnitude = -k * deltaL;
	f *= forceMagnitude;
	particle->addForce(f);
}

void ParticleSpring::updateK(float k_)
{
	k = k_;
}
