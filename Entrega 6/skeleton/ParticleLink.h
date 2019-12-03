#pragma once
#include "ParticleContact.h"

class ParticleLink
{
	public:
		//Holds the pair of particles that are connected by this link.
		Particle* particle[2];
	protected:
		float currentLength() const;
	public:
		virtual unsigned addContact(ParticleContact* contact) const = 0;
};

