#pragma once
#include <queue>
#include "ParticleContact.h"
//Particle ya esta incluido


class ParticleContactManager
{
	public:
		ParticleContactManager();
		virtual void update(float t);
		void addContact(ParticleContact* contact);
		~ParticleContactManager();

protected:
	std::queue<ParticleContact*> contactQ;
};

