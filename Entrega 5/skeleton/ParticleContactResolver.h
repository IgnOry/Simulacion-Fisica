#pragma once
#include "ParticleContact.h"

class ParticleContactResolver
{
protected:
	/**
	* Holds the number of iterations allowed.
	*/
	unsigned iterations;
	/**
	* This is a performance tracking value - we keep a record
	* of the actual number of iterations used.
	*/
	unsigned iterationsUsed;
public:
	/**
* Creates a new contact resolver.
*/
	ParticleContactResolver(unsigned iterations);
	/**
	* Sets the number of iterations that can be used.
	*/
	void setIterations(unsigned iterations);
	/**
	* Resolves a set of particle contacts for both penetration
	* and velocity.
	*/
	void resolveContacts(ParticleContact* contactArray,
		unsigned numContacts,
		float duration);
};

