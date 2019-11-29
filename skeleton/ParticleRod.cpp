#include "ParticleRod.h"

unsigned ParticleRod::addContact(ParticleContact* contact, unsigned limit) const
{
	// Find the length of the rod.
	float currentLen = currentLength();
	// Check if we’re overextended or overcompressed
	if (currentLen == length)
	{
		return 0;
	}
	// Otherwise, return the contact.
	// Calculate the normal.
	// The contact normal depends on whether we’re extending or compressing
	// Always use zero restitution (no bounciness).
	return 1;
}
