#include "PxGenerator.h"

PxGenerator::PxGenerator(Vector3 pos, PxScene* PhysxScene, PxPhysics* pxPhy): Particle(1.0, Vector4(1,1,1,1), pos, 50)
{
	forcesRegistry = new ParticleForceRegistry();
	scene = PhysxScene;
	PxSystem = pxPhy;
	spawnRateTime = 1;
}

PxGenerator::~PxGenerator()
{
}

void PxGenerator::update(float time)
{
	if (particlesVec.size() < limit && time_ > spawnRateTime)
	createParticle(time);

	auto it = particlesVec.begin();
	while (!particlesVec.empty() && it != particlesVec.end())
	{
		Particle* p = (*it);

		forcesRegistry->PxUpdateForces(time);

		it++;
	}

	time_ += time;
}

void PxGenerator::createParticle(float time)
{
	particlesVec.push_back(new Particle(0.5, position, scene, PxSystem, true, 0));
	time_ = 0;
}
