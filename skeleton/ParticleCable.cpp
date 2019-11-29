#include "ParticleCable.h"

unsigned ParticleCable::addContact(ParticleContact* contact, unsigned limit) const
{
	// Find the length of the cable.
	float length = currentLength();
	// Check if we’re overextended.
	if (length < maxLength)
	{
		return 0;
	}
	// Otherwise, return the contact.
	// Calculate the normal.
	// Add information to contact: normal, penetration & restitution.
	return 1;
}
