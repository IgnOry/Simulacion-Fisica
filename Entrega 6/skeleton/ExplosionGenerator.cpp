#include "ExplosionGenerator.h"
#include <iostream>

ExplosionGenerator::ExplosionGenerator(const Vector3 pos_, float radio_)
{
	particle = new Particle(radio_, Vector4(1.0, 1.0, 1.0, 0.0), pos_, 15.0);
	particle->setDirVel(Vector3(0, 0, 0), Vector3(0, 0, 0));
	radio = radio_;
}

ExplosionGenerator::~ExplosionGenerator()
{
	delete particle;
}

void ExplosionGenerator::updateForce(Particle* particleEx, float t)
{
	particle->update(t);

	Vector3 pos1 = particle->getPosition();
	Vector3 pos2 = particleEx->getPosition();

	float d = distance(particle->getPosition(), particleEx->getPosition());
	
	Vector3 dif = pos2 - pos1;
	
	force = dif.normalize();
	
	if (d < radio)
	{
		if (!particleEx->hasInfiniteMass())
		{
			particleEx->addForce(dif * force);
		}
	}
}

void ExplosionGenerator::PxUpdateForce(Particle* particleEx, float t)
{
	particle->update(t);

	Vector3 pos1 = particle->getPosition();
	Vector3 pos2 = particleEx->getDin()->getGlobalPose().p;

	float d = distance(pos1, pos2);

	Vector3 dif = pos2 - pos1;

	force = dif.normalize();

	if (d < radio)
	{
		particleEx->PxAddForce(dif * force);
	}
}
