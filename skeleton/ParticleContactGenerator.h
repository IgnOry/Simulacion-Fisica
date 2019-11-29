#pragma once
#include "ParticleContact.h"

class ParticleContactGenerator: public ParticleContact
{
	public:
	
	void resolveContacts(ParticleContact* contactArray, unsigned numContacts, float t);

	protected:
	
	//Probably moved
	int iterations;
	int iterationsUsed;
	int maxIndex;
};

