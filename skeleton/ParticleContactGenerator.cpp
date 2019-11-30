#include "ParticleContactGenerator.h"

void ParticleContactGenerator::resolveContacts(ParticleContact* contactArray, unsigned numContacts, float t)
{
	iterationsUsed = 0;
	while (iterationsUsed < iterations)
	{
		// Find the contact with the largest closing velocity;
		// Do we have anything worth resolving?
		if (maxIndex == numContacts) break;
		// Resolve this contact
		// Update the interpenetrations for all particles
		iterationsUsed++;
	}
}
