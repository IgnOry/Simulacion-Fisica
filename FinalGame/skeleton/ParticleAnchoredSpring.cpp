#include "ParticleAnchoredSpring.h"

void ParticleAnchoredSpring::updateForce(Particle* particle, float t)
{
	Vector3 f = particle->getPosition();
	f -= *anchor;
	// Length
	float length = f.normalize();
	// Resulting force
	f *= -((length - restLength) * k);
	particle->addForce(f);
}
