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

	Vector3 dif = pos1 - pos2;

	force = dif.normalize();

	/*std::cout << pos1.x << " " << pos1.y << " " << pos1.z << std::endl;
	std::cout << pos2.x << " " << pos2.y << " " << pos2.z << std::endl;
	std::cout << d << std::endl;*/


	//std::cout << radio << std::endl;

	if (d < radio)
	{
		if (!particleEx->hasInfiniteMass())
		{
			particleEx->addForce(dif * force);
		}
	}
}

void AttachForceGenerator::PxUpdateForce(Particle* particleEx, float t)
{
}

Particle* AttachForceGenerator::getParticle()
{
	return particle;
}
