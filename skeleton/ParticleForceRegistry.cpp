#include "ParticleForceRegistry.h"

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
	registrations.push_back({ particle, fg });
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
{
	int i = 0;
	while (i < registrations.size() && (registrations[i].particle != particle || registrations[i].fg != fg))
	{
		i++;
	}

	if (i < registrations.size()) 
	{
		registrations.erase(registrations.begin() + i);
	}
}

void ParticleForceRegistry::clear()
{
	registrations.clear();
}

void ParticleForceRegistry::updateForces(float t)
{
	for (auto it = registrations.begin(); it != registrations.end(); ++it)
	{
		it->fg->updateForce(it->particle, t);
	}
}

void ParticleForceRegistry::PxUpdateForces(float t)
{
	for (auto it = registrations.begin(); it != registrations.end(); ++it)
	{
		it->fg->PxUpdateForce(it->particle, t);
	}
}
