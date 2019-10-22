#include "WindGenerator.h"

WindGenerator::WindGenerator(const Vector3 wind_, const Vector3 pos_, float radio_)
{
	particle = new Particle (radio_, Vector4(1.0, 1.0, 1.0, 0.0), pos_, 15.0);
	particle->setDirVel(Vector3(0, 0, 0), Vector3(0, 0, 0));
	wind = wind_;
	radio = radio_;
}

void WindGenerator::updateForce(Particle* particleEx, float t)
{
	particle->update(t);

	if (distance(particle->getPosition(), particleEx->getPosition()) < radio)
	{
		if (!particleEx->hasInfiniteMass())
			particleEx->addForce(wind);
	}
}

WindGenerator::~WindGenerator()
{
	delete particle;
}
