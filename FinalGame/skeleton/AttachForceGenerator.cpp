#include "AttachForceGenerator.h"

AttachForceGenerator::AttachForceGenerator(float force_, Particle* planet_, float time_)
{
	planet = planet_;
	radio = planet->getRadio() * 2;
	particle = new Particle(radio, Vector4(1.0, 1.0, 1.0, 0.0), planet->getPosition(), time_);

	pos = particle->getPosition();
	force = force_;
}

AttachForceGenerator::~AttachForceGenerator()
{
	delete particle;
}

void AttachForceGenerator::updateForce(Particle* particleEx, float t)
{
	particle->update(t);

	/////////

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

void AttachForceGenerator::PxUpdateForce(Player* particleEx, float t)
{
	Vector3 pos1 = particle->getPosition();
	Vector3 pos2 = particleEx->getParticle()->getDin()->getGlobalPose().p;//->getPosition();

	//float d = distance(particle->getPosition(), particleEx->getDin()->getGlobalPose().p);

	Vector3 dif = pos2 - pos1;
	float d = dif.magnitude();

	force = dif.normalize();


	if (d < radio)
	{
		if (!particleEx->getParticle()->hasInfiniteMass())
		{
			particleEx->setTrapped(true);
			particleEx->getParticle()->getDin()->setLinearVelocity(-dif * force);
		}
	}
	else
		particleEx->setTrapped(false);
}

Particle* AttachForceGenerator::getParticle()
{
	return particle;
}
