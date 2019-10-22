#include "ParticleGravity.h"

void ParticleGravity::updateForce(Particle* particle, float t)
{
	if (particle->hasInfiniteMass()) return;
	particle->addForce(g * particle->getMass());
}
