#pragma once
#include "ParticleLink.h"

class ParticleRod: public ParticleLink
{
public:
	float length;
public:
	virtual unsigned addContact(ParticleContact* contact) const;
};

