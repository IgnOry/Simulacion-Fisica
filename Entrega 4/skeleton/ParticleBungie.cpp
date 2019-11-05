#include "ParticleBungie.h"

void ParticleBungee::updateForce(Particle* particle, float t)
{
	Vector3 f = particle->getPosition();
	f -= other->getPosition();
	// Length
	float length = f.normalize();
	length = (length - restLength);
	if (length <= 0.0f)
		return;
	// Resulting force
	f *= -(length * k);
	particle->addForce(f);
}
