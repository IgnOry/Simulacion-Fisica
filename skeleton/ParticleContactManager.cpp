#include "ParticleContactManager.h"

ParticleContactManager::ParticleContactManager()
{

}

void ParticleContactManager::update(ParticleContact* contact, float t)
{
	contact->resolve(t);
}

ParticleContactManager::~ParticleContactManager()
{
}
