#include "ParticleBuoyancy.h"

void ParticleBuoyancy::updateForce(Particle* particle, float t)
{
	float depth = particle->getPosition().y;
	if (depth > (waterHeight + maxDepth))
		// Out of the water -> nothing to do
		return;
	Vector3 f(0.0f, 0.0f, 0.0f);
	if (depth <= (waterHeight - maxDepth))
	{ // Totally under the water
		f.y = liquidDensity * volume;
	}
	else
	{
		float depthExterior = waterHeight + maxDepth;
		float volumeFactor = (depth - depthExterior) / (2 * maxDepth);
		f.y = liquidDensity * volumeFactor * volume;
	}
	particle->addForce(f);
}