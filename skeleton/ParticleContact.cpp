#include "ParticleContact.h"

void ParticleContact::resolve(float t)
{
	resolveVelocity(t);
	resolveInterpenetration(t);
}

float ParticleContact::calculateSeparatingVelocity() const
{
	Vector3 relativeVelocity = particle[0]->getVelocity();
	if (particle[1])
		relativeVelocity -= particle[1]->getVelocity();
	return (relativeVelocity * contactNormal);}

void ParticleContact::resolveVelocity(float t)
{
	// Get current separating velocity
	// Compute separating velocity after collision
	// Compute total impulse
	// Compute impulse for each particle
	// Set new velocity for each particle
}

void ParticleContact::resolveInterpenetration(float t)
{
	// Check if there is interpenetration
	// Calculate factor of each particle depending on mass
	// Calculate the movement for each particle
	// Apply movement to the position of each particle
}
