#pragma once
#include "ParticleContact.h"
//Particle ya esta incluido


class ParticleContactManager
{
	public:
		ParticleContactManager();
		virtual void update(ParticleContact* contact, float t);
		~ParticleContactManager();
};

