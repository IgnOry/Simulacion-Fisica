#include "ExplosionGenerator.h"
#include <iostream>

ExplosionGenerator::ExplosionGenerator(const Vector3 pos_, float radio_)
{
	particle = new Particle(radio_, Vector4(1.0, 1.0, 1.0, 0.0), Vector3(pos_.x + 5, pos_.y + 5, pos_.z + 5), 15.0);
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
	
	float force;

	force = dif.normalize();
	
	if (d < radio)
	{
		if (!particleEx->hasInfiniteMass())
		{
			particleEx->addForce(dif * force);
		}
	}
}
