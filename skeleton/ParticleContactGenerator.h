#pragma once
#include "ParticleContact.h"

class ParticleContactGenerator
{
	public:
	
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
};

