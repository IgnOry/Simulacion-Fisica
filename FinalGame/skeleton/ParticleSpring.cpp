#include "ParticleSpring.h"

ParticleSpring::ParticleSpring(float k_, float restLength_, Vector3 pos)
{
	other = new Particle(1, Vector4(1, 0, 0, 1), pos, 50, 0, 0, 0);
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

float ParticleSpring::getK()
{
	return k;
}

void ParticleSpring::kkk(float k_)
{
	k += k_;
}

void ParticleSpring::setPosition(Vector3 pos)
{
	other->setPosition(pos);
	other->setRItem(1);
}

Particle* ParticleSpring::getOther()
{
	return other;
}
