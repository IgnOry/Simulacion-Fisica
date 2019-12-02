#include "ParticleContactManager.h"

ParticleContactManager::ParticleContactManager()
{

}

void ParticleContactManager::update(float t)
{

	while (!contactQ.empty()) {
		auto auxC = contactQ.front();

		ParticleContact* cont = (auxC);

		cont->resolve(t);

		contactQ.pop();
	}
}

void ParticleContactManager::addContact(ParticleContact* contact)
{
	contactQ.push(contact);
}

ParticleContactManager::~ParticleContactManager()
{
}
