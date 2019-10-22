#include "ParticleDrag.h"

void ParticleDrag::updateForce(Particle* particle, float t)
{
	Vector3 f;
	f = particle->getVelocity();
	// Drag coeffient
	float dragCoeff = f.normalize();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;
	// Final force
	f *= -dragCoeff;
	particle->addForce(f);
}
